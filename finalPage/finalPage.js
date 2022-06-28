$(document).ready(function() {
  final_feedback()
  setTimeout(function(){
    location.href="../startPage/startPage.html";
  },30000) //피드백 30초(millis 단위라 1000이 1초)??
});

{ //명언
  var myTest = new Array(
          '삶이 있는 한 희망은 있다 - 키케로', 
          '산다는것 그것은 치열한 전투이다 - 로망로랑', 
          '피할수 없으면 즐겨라 – 로버트 엘리엇', 
          '한번의 실패와 영원한 실패를 혼동하지 마라 - F.스콧 핏제랄드', 
          '행복은 습관이다. 그것을 몸에 지니라 - 허버드', 
          '오랫동안 꿈을 그리는 사람은 마침내 그 꿈을 닮아 간다 - 앙드레 말로');

document.getElementById("comment1").innerHTML = randomItem(myTest);

function randomItem(a) {
return a[Math.floor(Math.random() * a.length)];
  }
}

var settings = {
  "url": "http://203.253.128.177:7579/Mobius/fitco/time/la",
  "method": "GET",
  "timeout": 0,
  "headers": {
    "Accept": "application/json",
    "X-M2M-RI": "12345",
    "X-M2M-Origin": "SOrigin"
  },
};
function final_feedback(){
  $.ajax(settings).done(function (response) {
      var cinval = response["m2m:cin"]; 
      var conval = cinval["con"];
      console.log(conval); //conval에 time들어있음
     
      //현재 총 이용 시간
      var spconval = conval.split(','); //분할
      var firstcon = spconval[0];
      var lastcon = spconval[spconval.length - 1];
      var nowtime = ((lastcon - firstcon) / 1000) / 60; //분단위로 표시하려고 60으로 다시 나눔
      document.getElementById("comment2").innerHTML = "현재 총 이용시간 : " + "6.23" + "분";

      //평균 값 구하기
      const Avertime = [6, 4, 0];
      if(Avertime[Avertime.length-1] != nowtime) {
        var tt = nowtime.toFixed(0);
        tt *= 1; //스트링 숫자로 변환(js 자동 변환은 숫자 곱해주면 형 바꿔줌)
        Avertime.push(tt);
      }
      const sum = Avertime.reduce((a,b) => (a+b)); //더해서
      const average = sum / Avertime.length; //평균내줌
      document.getElementById("result_time").innerHTML = average.toFixed(2) + "분";

      //이전 시간 - 현재 시간
      var minAvertime = Avertime[Avertime.length-2] - Avertime[Avertime.length-1];
      document.getElementById("result_minus").innerHTML = minAvertime + "분";
      if(minAvertime >= 1) {
        document.getElementById("result_comment").innerHTML = '배변 활동에 도움을 주는</br> 운동을 해보는 것은 어떨까요?';
      }
      if(minAvertime < 0) {
        document.getElementById("result_comment").innerHTML = '저번보다 시간이 줄었어요!';
      }
      if(minAvertime == 0) {
        document.getElementById("result_comment").innerHTML = '시간의 큰 변동은 없네요!';
      }

      //document.getElementById("next").innerHTML += conval; 
  
      // var result_Avertime = 0;
      // for (var i = 0; i < Avertime.length; i++){
      //   result_Avertime += Avertime[i];
      // }
      // console.log(result_Avertime);
      // console.log(Avertime.length);
      // console.log(result_Avertime / Avertime.length);
  
    });
  
}






  



  
