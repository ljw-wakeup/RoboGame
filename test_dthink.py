#!/usr/bin/env python3.6
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
while(1)
    n=input("witch one?\n")
    if n==8:
        break
    GPIO.setup(n, GPIO.IN)
    a = GPIO.input(n)
    print(a+"\n")
GPIO.cleanup()
