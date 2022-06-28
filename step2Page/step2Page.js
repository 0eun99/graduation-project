var constat = '6';

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


// var settings = {
//   "url": "http://203.253.128.177:7579/Mobius/fitco/user/la",
//   "method": "GET",
//   "timeout": 0,
//   "headers": {
//     "Accept": "application/json",
//     "X-M2M-RI": "12345",
//     "X-M2M-Origin": "SOrigin"
//   },
// };



// var settings_Y = {
//   "url": "http://203.253.128.177:7579/Mobius/fitco/result_label_Y/la",
//   "method": "GET",
//   "timeout": 0,
//   "headers": {
//     "Accept": "application/json",
//     "X-M2M-RI": "12345",
//     "X-M2M-Origin": "SOrigin"
//   },
// };

// var settings_C = {
//   "url": "http://203.253.128.177:7579/Mobius/fitco/result_label_C/la",
//   "method": "GET",
//   "timeout": 0,
//   "headers": {
//     "Accept": "application/json",
//     "X-M2M-RI": "12345",
//     "X-M2M-Origin": "SOrigin"
//   },
// };




// function updateData(){
//   $.ajax(settings).done(function (response) {
//     var cinuser = response["m2m:cin"]; 
//     var conuser = cinuser["con"];
//     console.log(conuser); //콘솔에 tt2출력(f12)

//     if (conuser == 'Y'){
//       var set =  settings_Y;
//     }else{
//       var set =  settings_C;
//     }

//     loadPage(conuser);
//   });
// }


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
        document.getElementById("next").innerHTML += constat;
        if(label=='2'){
          constat = 2;
          location.href="../goodPage/goodPage.html";
        }
        else if(label=='3'){
          constat = 3;
          location.href="../commentPage/comment3.html";
        }
        else if(label=='4'){
          constat = 4;
          location.href="../againPage/againPage.html";
        }
        else if(label=='5'){
          constat = 5;
          location.href="../step1Page/step1Page.html";
        }
        else if(label=='7'){
          constat = 7;
          location.href="../step3Page/step3Page.html";
        }
        
      }
    }else if(button == '0'){
      location.href="../finalPage/finalPage.html";
    }
    else if(button == '6'){
      location.href="../timeoutPage/timeout.html";
    }
  });
}









  



  
