var constat = '2';

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
    var cinval = response["m2m:cin"]; 
    var conval = cinval["con"];
    console.log(conval); //콘솔에 tt2출력(f12)

    conval = conval.split(',')
    label = conval[0] //레이블
    time =  conval[1]//시간
    button = conval[2]//버튼
    // document.getElementById("next").innerHTML += button; 
    
    //버튼 값이 1이면 페이지 넘김(조언 페이지로)
    // if (button == '1'){
    //   // document.getElementById("next").innerHTML += button;  
    //   if (constat != label){
    //     document.getElementById("next").innerHTML += constat;
    //     if(label=='1'){
    //       constat = 1;
    //       location.href="../commentPage/comment1.html";
    //     }
    //     else if(label=='3'){
    //       constat = 3;
    //       location.href="../commentPage/comment3.html";
    //     }
    //     else if(label=='4'){
    //       constat = 4;
    //       location.href="../againPage/againPage.html";
    //     }
        
    //   }
    // }else if(button == '0'){
    //   // 3초동안 타임아웃 페이지 보여준뒤 final페이지로
      setTimeout(function(){
        location.href="../finalPage/finalPage.html";
      },3000)
    // }
  });
}









  



  
