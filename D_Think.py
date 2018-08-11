#!/usr/bin/env python3.6
def read_dthink():#读取灰度传感器的八位数，并用list返回
    GPIO.setmode(GPIO.BOARD)
    a = [0, 1, 2, 3, 4, 5, 6, 7]
    L = [];
    for i in a:
       GPIO.setup(i, GPIO.IN)
    for i in a:
        L.append(GPIO.input(i))
    return L
