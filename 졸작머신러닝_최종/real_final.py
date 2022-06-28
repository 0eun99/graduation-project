# -*- coding: utf-8 -*-
# import pandas as pd
# import numpy as np 

# test='0'
# df = pd.DataFrame(columns=['time'])
# # df.loc[len(df)] = toTEST

# for i in range(10):
#     df.loc[len(df)] = i
    
   
# print(df)
from threading import Thread
from time import sleep
import requests
import pandas as pd
import numpy as np 
import re
import csv
import json
import os

#일단 전체 출력
url1 = "http://203.253.128.177:7579/Mobius/fitco/hi/la"
payload1={}
headers1 = {
  'Accept': 'application/json',
  'X-M2M-RI': '12345',
  'X-M2M-Origin': 'SOrigin'
}

#poset_user
url0 =  "http://203.253.128.177:7579/Mobius/fitco/user"
headers = {
  'Accept': 'application/json',
  'X-M2M-RI': '12345',
  'X-M2M-Origin': '{{aei}}',
  'Content-Type': 'application/vnd.onem2m-res+json; ty=4'
}


#사용자 별 url들
#유진언니---------------------------------------------------------------
# url_result_Y = "http://203.253.128.177:7579/Mobius/fitco/result_label_Y"
# url_time_Y = "http://203.253.128.177:7579/Mobius/fitco/time_Y"


#영은-------------------------------------------------------------------
# url_result_C = "http://203.253.128.177:7579/Mobius/fitco/result_label_C"
# url_time_C = "http://203.253.128.177:7579/Mobius/fitco/time_C"


#사용자 구별 x url들-----------------------------------------------------
url_result = "http://203.253.128.177:7579/Mobius/fitco/result_label"
url_time = "http://203.253.128.177:7579/Mobius/fitco/time"




#사용자 포스트
def post_user(user):
  # print(type(total))
  payload0 = '{\n    \"m2m:cin\": {\n        \"con\":'+'\"' +user+ '\"'+'\n    }\n}'

  response0 = requests.request("POST", url0, headers=headers, data=payload0)
  print('success post(user) :',user)



def post(total, user):
  payload2 = '{\n    \"m2m:cin\": {\n        \"con\":'+'\"' +total+ '\"'+'\n    }\n}'

  response2 = requests.request("POST", url_result, headers=headers, data=payload2)
  # if user == 'Y': response2 = requests.request("POST", url_result_Y, headers=headers, data=payload2)
  # elif user == 'C': response2 = requests.request("POST", url_result_C, headers=headers, data=payload2)
  print('success post(result_label) :',total)


def post_time(time):
    payload3 = '{\n    \"m2m:cin\": {\n        \"con\":'+'\"' +time+ '\"'+'\n    }\n}'
    
    response2 = requests.request("POST", url_time, headers=headers, data=payload3)
    # if user == 'Y': response2 = requests.request(s"POST", url_time_Y, headers=headers, data=payload3)
    # elif user == 'C': response2 = requests.request("POST", url_time_C, headers=headers, data=payload3)

    print('success post_all_time :')


def get_con():
    response1 = requests.request("GET", url1, headers=headers1, data=payload1)
    t = response1.text
    json_data = json.loads(t)
    m2mcin = json_data["m2m:cin"]
    con = m2mcin["con"]
    con= con.split(';')
    return list(con)


from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from collections import Counter

#시간 저장할 전역변수
time=''

print('wait button value 1')
while True:
  con = get_con()
  #print(con[1])
  user = 0

  if con[1] =='1':
      while True:
          #추가할 센서값 가져오기---------------------------------------------------------------
          con = get_con()
          print('classifiying..')

          # #피드백js에서 끌어올것
          button = con[1]
          time_val=con[0]
          
          time += time_val+','
          #시간, 버튼 저장
          time_btn = con[0]+','+con[1] 

          del con[:2]
          del con[-1]

          print('###')
          # print(time_btn)
          # print(type(con[0]))

          df = pd.DataFrame(columns=['load1','load2','load3','load4','load5', 'load5', 'load7'])

          #받은 test(검증 값) 데이터프레임에 담기
          for value_list in con:
              toTEST = value_list.split(',')
              # print(toTEST)
              df.loc[len(df)] = toTEST
          # print(df)

          #test해볼 값들
          test_set = df
          # print(test_set)
          
          #머신러닝 시작----------------------------------------------------------------------
          data = pd.read_csv('CH_Yj.csv')

          #train데이터, 레이블을 분리
          train_data = data.drop(["label"], axis=1)
          label_data = data['label'].copy()

          x_train, x_test, y_train, y_test = train_test_split(train_data, label_data, test_size=0.2, random_state=42)


          # #RamdomForestClasdsifier
          rnd_clf = RandomForestClassifier(n_estimators=200, random_state=42)
          rnd_clf.fit(x_train, y_train )
          rnd_predict = rnd_clf.predict(test_set)
          print('-predict:',rnd_predict)
          rnd_pred = rnd_clf.predict_proba(test_set)
          # print(rnd_pred)

          count_label = Counter(rnd_predict)
          result = count_label.most_common(1)
          result_label = str(result[0][0])
          print('-result_label:',result_label)
          
          #사용자 선택
          if user == 0:
              user = result_label[0]
              print('user is: ', user,'\n')
              post_user(user)

          ## (레이블, 시간, 버튼) 한번에 저장
          ## time_btn = 시간, 버튼 들어있
          total = result_label[1]+','+time_btn 
          # print(total)
          #버튼값이 0이면 while문 나감
          if button == '0':
              print('end button push')
              time  = time[:-2]
              post(total,user) #(레이블, 시간, 버튼)이랑 사용자 구별:user
              post_time(time)#(시간들만 보내는거)
              print('---finish---')
              break

          elif button =='6':
            total = result_label[1]+','+time_btn 
            print('---**timeout**---')
            time  = time[:-2]
            post(total,user)#(레이블, 시간, 버튼)이랑 사용자 구별:user
            post_time(time)#(시간들만 보내는거)
            print('finish')
            break
          #모비우스로 결과 보내기-----------------------------------------
          post(total, user)
