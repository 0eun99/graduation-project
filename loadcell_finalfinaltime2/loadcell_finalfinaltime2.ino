#include <SPI.h>
#include <WiFi101.h>
// MQTT protocol, nCube Arduino 
#include <string.h>
#include <stdlib.h>
#include <avr/dtostrf.h>
#include <Wire.h>
#include "Time.h" //시간 관련 
#include "HX711.h"


// connect to WiFi
char ssid[] = "ultrayoukim";      // your network SSID (name)
char pass[] = "kim6264$";             // your network password (use for WPA, or use as key for WEP)
WiFiClient client;


// Mobius server 
int status = WL_IDLE_STATUS;
IPAddress server(203, 253, 128, 177);   // server ip address -> Mobius IP
//IPAddress server(203, 253, 128, 161);   // SCH mobius


// I. Jeong 8/10-14
// ADD AE registration
// ADD AE registration
// ADD AE registration

String AE = "fitco";

// json data format
// cin json data format
//String cin_format_1 = "{\"m2m:cin\": {\"con\": [";
//String cin_format_2 = "]}}";

// J. Yun, POST in the form of a string not array
String cin_format_1 = "{\"m2m:cin\": {\"con\": \"";
String cin_format_2 = "\"}}";

int init_data_len = cin_format_1.length() + cin_format_2.length();
int sensing_data_len = 0; // length of pir_sensing data

// pir sensing data

// data length 
// const int arr_len = 600;    // 1 min (60 * 10 - 10hz)
const int arr_len = 10;  // debug

// sensing data 
char **pir_mat;
float *pir_log_time_arr;
float pir_log_time = 0;
int pir_log_idx = -1;

unsigned long pir_sensing_timer = 0; 
unsigned long pir_sensing_interval = 100;  // sensing every 100 milliseconds (10hz)

// loadcell pin num
const int loadpin1 = 12;
const int loadpin2 = 13;
const int loadpin3 = 10;
const int loadpin4 = 11;
const int loadpin5 = 6;
const int loadpin6 = 9;
const int loadpin7 = 20;
const int loadpin8 = 21;
const int loadpin9 = 0;
const int loadpin10 = 1;

//로드셀 발판 pin num
const int loadpin11 = A0;
const int loadpin12 = A1;
const int loadpin13 = A2;
const int loadpin14 = A3;

//button pin
const int btn = 22;
boolean lastButton LOW;
boolean currentButton = LOW;

//kg
float kg1 = 0;// 로드셀 1 값 받는 변수
float kg2 = 0; //로드셀 2 값 받는 변수
float kg3 = 0; //로드셀 3 값 받는 변수
float kg4 = 0; //로드셀 4 값 받는 변수
float kg5 = 0; //로드셀 5 값 받는 변수
float kg6 = 0; //로드셀 6 값 받는 변수
float kg7 = 0; //로드셀 7 값 받는 변수

// user input through serial transmission
int input = 0;
int sensing_flag = 0;

HX711 scale1; //로드셀 1
HX711 scale2; //로드셀 2
HX711 scale3; //로드셀 3
HX711 scale4;// 로드셀 4
HX711 scale5;// 로드셀 5
//로드셀 6, 7 발판
HX711 scale6;// 로드셀 6
HX711 scale7;// 로드셀 7

float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup

//타이머 
const int interruptPin = 2;//인터럽트핀
unsigned long previousMillis = 0; //이전시간
unsigned long currentMillis = 0; //현재시간
const long interval = 6000; //6초마다 타이머작동 


void setup() {
     
    // put your setup code here, to run once:
    Serial.begin(9600);
    
    pinMode(interruptPin, INPUT_PULLUP);//타이머셋업 
    
    scale1.begin(loadpin1, loadpin2); //로드셀 1
    scale1.set_scale(); //로드셀 1
    scale1.tare(); //Reset the scale to 0//로드셀 1

    scale2.begin(loadpin3, loadpin4); //로드셀 2
    scale2.set_scale(); //로드셀 2
    scale2.tare(); //Reset the scale to 0//로드셀 2

    scale3.begin(loadpin5, loadpin6); //로드셀 3
    scale3.set_scale(); //로드셀 3
    scale3.tare(); //Reset the scale to 0//로드셀 3

    scale4.begin(loadpin7, loadpin8); //로드셀 4
    scale4.set_scale(); //로드셀 4
    scale4.tare(); //Reset the scale to 0//로드셀 4
    
    scale5.begin(loadpin9, loadpin10); //로드셀 5
    scale5.set_scale(); //로드셀 5
    scale5.tare(); //Reset the scale to 0//로드셀 5

    scale6.begin(loadpin11, loadpin12); //로드셀 6
    scale6.set_scale(); //로드셀 6
    scale6.tare(); //Reset the scale to 0//로드셀 6

    scale7.begin(loadpin13, loadpin14); //로드셀 7
    scale7.set_scale(); //로드셀 7
    scale7.tare(); //Reset the scale to 0//로드셀 7

    pinMode(btn, INPUT); //버튼 input

    WiFi.setPins(8,7,4,2);
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        while (true);
    }
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid); 
        //status = WiFi.begin(ssid);  // don't need wifi password
        status = WiFi.begin(ssid, pass);  // need wifi password

        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println("Connected to wifi");
    printWiFiStatus();

    // assign memory 
    // pir_mat = (char **) malloc(sizeof(short*)*arr_len);
    pir_mat = (char **) malloc(sizeof(char*)*arr_len);
    for(int i =0; i<arr_len; i++){
        pir_mat[i] = (char*)malloc(sizeof(char)*48); 
        pir_mat[i][0] = 0;
    }
}

void initCharArr(){
  for(int i =0; i<arr_len; i++){
    pir_mat[i][0] = 0;
  }
}

void loop() {
    //사람이 앉았을 때 
    usersit();
    // key '0': start capture
    // key '1': stop capture
    userInput();
    
    if(sensing_flag == 1){
        pirSensing();
    }
    // get http response message from server
    if(client.available()) {
        char c = client.read();
        Serial.write(c);
    }
}

void userInput(){
//    currentMillis = millis();
//    if(currentMillis - previousMillis >= 30000 ){ //30초가 되면 권상시간초과 알람 
//            Serial.println("Timer count");
//            input = 6;
//            sensing_flag = 1;
//            previousMillis = currentMillis;
//    }

    //input = 0;
    //sensing_flag = 0;
    
    
         
         
    currentButton = digitalRead(btn); //버튼이 눌리면
      if(lastButton==LOW && currentButton==HIGH ) {
          if(input == 0){            // start pir sensing
              sensing_flag = 1;
              Serial.println("start sensing");
              input = 1;
          }else if(input == 1){      // stop pir sensing
              input = 0;
              if(pir_log_idx > -1){ 
                  Serial.println(pir_log_idx);     // debug
                  uploadAndInit();
              }else{
                  client.stop();
              }
              Serial.println("quit sensing");
              sensing_flag = 0;
          }
    }
    lastButton = currentButton;
}

void usersit() {
    scale1.set_scale(-7050); //Adjust to this calibration factor//로드셀 1
    scale2.set_scale(-7050); //Adjust to this calibration factor//로드셀 2
    scale3.set_scale(-7050); //Adjust to this calibration factor//로드셀 3
    scale4.set_scale(-7050); //Adjust to this calibration factor//로드셀 4
    scale5.set_scale(-7050); //Adjust to this calibration factor//로드셀 5
    scale6.set_scale(-7050); //Adjust to this calibration factor//로드셀 6
    scale7.set_scale(-7050); //Adjust to this calibration factor//로드셀 7
    kg1 = abs(scale1.get_units()*0.453592);//로드셀 1 절대값으로
    kg2 = abs(scale2.get_units()*0.453592);//로드셀 2
    kg3 = abs(scale3.get_units()*0.453592);//로드셀 3
    kg4 = abs(scale4.get_units()*0.453592);//로드셀 4
    kg5 = abs(scale5.get_units()*0.453592);//로드셀 5
    kg6 = abs(scale6.get_units()*0.453592);//로드셀 6
    kg7 = abs(scale7.get_units()*0.453592);//로드셀 7

    //값이 0 이상이면 시작, 그 안에서 만약 전부 0이되면 멈추게
    if(kg1>1 || kg2>1 || kg3>1 || kg4>1 || kg5>1 || kg6>1 || kg7>1){
        sensing_flag = 1;
        Serial.println("start usersit");
        //input = 1;

        currentMillis = millis();
        if(currentMillis - previousMillis >= 3000 ){ //30초가 되면 권상시간초과 알람 
                Serial.println("Timer count");
                input = 6;
                //sensing_flag = 1;
                previousMillis = currentMillis;
        }


        
    } else if(kg1 < 1 && kg2 < 1 && kg3 < 1 && kg4 < 1 && kg5 < 1 && kg6 < 1 && kg7 < 1){ 
              input = 0;
              previousMillis = 0;
              currentMillis = 0;
              if(pir_log_idx > -1){ 
                  Serial.println(pir_log_idx);     // debug
                  uploadAndInit();
              }else{
                  client.stop();
              }
              //Serial.println("quit usersit");
              sensing_flag = 0;  
      }   
    
}


void postMobius(){
    client.stop();
    
    if(pir_log_idx != arr_len-1){
        int len = strlen(pir_mat[pir_log_idx]);
        pir_mat[pir_log_idx][len-1] = 0;
        sensing_data_len = sensing_data_len-1;
        
        Serial.print(pir_mat[pir_log_idx]);
    }
    
    int data_len = init_data_len + (sensing_data_len);

    if(client.connect(server, 7579)){
        Serial.println("connected to server");
        
        // Need to insert secure communication methods
        // Need to insert secure communication methods
        // Need to insert secure communication methods


        // request line        
        // change ae and cnt name here
        client.println("POST /Mobius/fitco/hi HTTP/1.1");

        // header
        client.println("Host: 203.253.128.177:7579");
        client.println("Accept: application/json");
        client.println("X-M2M-RI: 12345");
        client.println("X-M2M-Origin:SOrigin");
        client.print("Content-Length: ");

        // 여기 새로 추가됨 J. Yun, time in Arduino (transmitted, not recorded)
        char t[30];
        sprintf(t, "%lu;\0,", millis());
        data_len = data_len + strlen(t);
//        Serial.println(data_len);

        //버튼 값 붙여서 보내기 -> 타이머로 변경 
        char num[5];
        sprintf(num, "%d;\0", input);
        data_len = data_len + strlen(num);
       
        client.println(data_len);
        client.println("Content-Type: application/vnd.onem2m-res+json; ty=4");
        client.println();

        // write data
        client.print(cin_format_1);

        // J. Yun, write data transmission time 여기 새로 추가됨
        client.print(t);
        client.print(num);
        
        for(int i =0; i<pir_log_idx+1; i++){
            client.print(pir_mat[i]);
        }

        client.print(cin_format_2);
        client.println();

        Serial.println("sended post request to Mobius");
        sensing_data_len = 0;
    }
}


char pir_temp[7][20];

void pirSensing(){
    if(millis() - pir_sensing_timer > pir_sensing_interval){
        pir_sensing_timer = millis();
        pir_log_idx ++;
        pir_log_time += 0.1;
        pir_log_time_arr[pir_log_idx] = pir_log_time;
    
        scale1.set_scale(-7050); //Adjust to this calibration factor//로드셀 1
        scale2.set_scale(-7050); //Adjust to this calibration factor//로드셀 2
        scale3.set_scale(-7050); //Adjust to this calibration factor//로드셀 3
        scale4.set_scale(-7050); //Adjust to this calibration factor//로드셀 4
        scale5.set_scale(-7050); //Adjust to this calibration factor//로드셀 5
        scale6.set_scale(-7050); //Adjust to this calibration factor//로드셀 6
        scale7.set_scale(-7050); //Adjust to this calibration factor//로드셀 7
        kg1 = abs(scale1.get_units()*0.453592);//로드셀 1 절대값으로
        kg2 = abs(scale2.get_units()*0.453592);//로드셀 2
        kg3 = abs(scale3.get_units()*0.453592);//로드셀 3
        kg4 = abs(scale4.get_units()*0.453592);//로드셀 4
        kg5 = abs(scale5.get_units()*0.453592);//로드셀 5
        kg6 = abs(scale6.get_units()*0.453592);//로드셀 6
        kg7 = abs(scale7.get_units()*0.453592);//로드셀 7

        sprintf(pir_temp[0], "%d,", (int)kg1);
        sprintf(pir_temp[1], "%d,", (int)kg2);
        sprintf(pir_temp[2], "%d,", (int)kg3);
        sprintf(pir_temp[3], "%d,", (int)kg4);
        sprintf(pir_temp[4], "%d,", (int)kg5);
        sprintf(pir_temp[5], "%d,", (int)kg6); //발판1
        sprintf(pir_temp[6], "%d;\0", (int)kg7); //발판2

        for(int i = 0; i<7; i++){
            strcat(pir_mat[pir_log_idx], pir_temp[i]);
        }
        if(pir_log_idx < arr_len-1)
            strcat(pir_mat[pir_log_idx], "\0");

        Serial.println(pir_mat[pir_log_idx]);

        sensing_data_len += strlen(pir_mat[pir_log_idx]);
    
        Serial.println(pir_log_idx);
        if(pir_log_idx >= arr_len-1){
            Serial.println("post req");
            uploadAndInit();
        }
    }
}

void uploadAndInit(){
    postMobius();
    pir_log_idx = -1;
    initCharArr();
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
