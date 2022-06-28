# -*- coding: utf-8 -*-
import RPi.GPIO as GPIO 
import time 
import datetime
import threading

#con
import requests
#import pandas as pd
#import numpy as np
import re
import json
import os


#이게 가장 최근 코드 6분 타이머 추가한 것


#코드 시작
while True:

    url = "http://203.253.128.177:7579/Mobius/fitco/result_label/la"
    payload={}
    headers = {
    'Accept': 'application/json',
    'X-M2M-RI': '12345',
    'X-M2M-Origin': 'SOrigin'
    }

    response = requests.request("GET", url, headers=headers, data=payload)

    t = response.text
    # print(t)

    #json data state
    json_data = json.loads(t)
    # print(json_data)

    #con value
    m2mcin = json_data["m2m:cin"]
    con = m2mcin["con"]
    # print(con)
    #con content: label, time, button_value !!

    #label
    label_stat = con[0]
    now_label = label_stat
    btn = con[-1]
    ##print(label_stat)
    ##print(btn)
    ##print('first')

    #앉기 전 시간 읽기
    time_back = time.time()

    #앉았을 때 자세 바꾸기, n초 경과시 알람 울림 시작
    while btn == '1':
        #현재 시간 읽어오기
        time_now = time.time()

        #앉기 직전 시간과 현재값 차이 계산
        total_time = time_now - time_back
        if total_time > 20 : #n초 경과시 부저울림(시간 변경하고 싶으면 숫자 바꿔야함, 지금은 20초마다 울리게 해놓음)
            print('timeover')
            total_time = 0
            time_back = time_now
            GPIO.setmode(GPIO.BCM) 
            gpio_pin = 23 #gpio pin number
            GPIO.setup(gpio_pin, GPIO.OUT)
            GPIO.setwarnings(False)
            pwm = GPIO.PWM(gpio_pin, 0.1) # '100' is sound frequency
            pwm.start(50.0)
            
            for cnt in range(0,3):
                pwm.ChangeFrequency(262)
                time.sleep(0.1)
                pwm.ChangeFrequency(294)
                time.sleep(0.1)
                pwm.ChangeFrequency(330)
                time.sleep(0.1)
            pwm.ChangeDutyCycle(0.0)    
            pwm.stop()
            GPIO.cleanup()

        ##print('hi')
        #자세 바뀌는 값 측정하기 위해 계속 읽어오기
        url = "http://203.253.128.177:7579/Mobius/fitco/result_label/la"
        payload={}
        headers = {
        'Accept': 'application/json',
        'X-M2M-RI': '12345',
        'X-M2M-Origin': 'SOrigin'
        }

        response = requests.request("GET", url, headers=headers, data=payload)

        t = response.text
        # print(t)

        #json data state
        json_data = json.loads(t)
        # print(json_data)

        #con value
        m2mcin = json_data["m2m:cin"]
        con = m2mcin["con"]
        # print(con)
        #con content: label, time, button_value !!


        #label
        label_stat = con[0]
        btn = con[-1]
        ##print(label_stat)
        ##print(btn)
        ##print('second')

        back_label = now_label
        now_label = label_stat
        ##print(back_label)
        ##print(now_label)
        ##print('third')
        #이전 자세와 현재 자세가 다를 경우 부저울리기
        if(back_label != label_stat):
            print('gogo')
            GPIO.setmode(GPIO.BCM) 
            gpio_pin = 23 #gpio pin number
            GPIO.setup(gpio_pin, GPIO.OUT)
            GPIO.setwarnings(False)
            pwm = GPIO.PWM(gpio_pin, 292) # '262' is sound frequency
            pwm.start(50.0)
            time.sleep(0.1) # 1.5times sound keep
            pwm.stop()
            GPIO.cleanup()
