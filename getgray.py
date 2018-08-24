#!/usr/bin/env python3.6
import time
import RPi.GPIO as GPIO

def read_gray():#读取灰度传感器的八位数，并用list返回
    GPIO.setmode(GPIO.BOARD)
    a = [7,12,11,16, 13, 18, 15, 22]
    L = [];
    for i in a:
       GPIO.setup(i, GPIO.IN)
    for i in a:
        L.append(GPIO.input(i))
    return L


