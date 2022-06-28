var constat = '0';

var label = '0';
var time =  '0';
var button = '0';
var cinval = '0';
var conval = '0';

$(document).ready(function() {
  setInterval(function(){
    updateData();
  },1000)
}); 



var settings = {
  "url": "http://203.253.128.177:7579/Mobius/fitco/result_label/la",
  "method": "GET",
  "timeout": 0,
  "headers": {
    "Accept": "application/json",
    "X-M2M-RI": "12345",
    "X-M2M-Origin": "SOrigin"
  },
};


function updateData(){
  $.ajax(settings).done(function (response) {
    cinval = response["m2m:cin"]; 
    conval = cinval["con"];
    console.log(conval); //콘솔에 tt2출력(f12)

    conval = conval.split(',')
    label = conval[0] //레이블
    time =  conval[1]//시간
    button = conval[2]//버튼
    // document.getElementById("next").innerHTML += button; 
    
    //버튼 값이 1이면 페이지 넘김(조언 페이지로)
    if (button == '1'){
      // document.getElementById("next").innerHTML += button;  
      if (constat != label){
  
        if(label=='1'){
          constat = 1;
          document.getElementById("next").innerHTML += constat;
          location.replace("../commentPage/comment1.html");
        }
        else if(label=='2'){
          constat = 2;
          location.replace("../goodPage/goodPage.html");
        }
        else if(label=='3'){
          constat = 3;
          location.replace("../commentPage/comment3.html");
        }
        else if(label=='4'){
          constat = 4;
          location.replace("../againPage/againPage.html");
        }
        else if(label=='5'){
          constat = 5;
          location.replace("../step1Page/step1Page.html");
        }
        else if(label=='6'){
          constat = 6;
          location.replace("../step2Page/step2Page.html");
        }
        else if(label=='7'){
          constat = 7;
          location.replace("../step3Page/step3Page.html");
        }
        
      }
    }
    // else if(button == '0'){
    //   location.href="../finalPage/finalPage.html";
    // }
    
  });
}



  



  
