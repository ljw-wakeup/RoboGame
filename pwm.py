import RPi.GPIO as GPIO
import time
import cacurlationForPid as Cpid
import pidFromGray as GPID
import getgray as getgray


def refreshmotor(dc,direction):
    input_gray = getgray.read_gray()

    status, output = Cpid.caculGrayForPid(input_gray)
    if status:
        a=GPID.pidFromGray(output)
    else:
        a=0
    for i in range(4):
        dc[0]=60 +a*20
        dc[1]=60 -a*20
    for i in range(4):
        if dc[i]>90:
            dc[i]=90
        elif dc[i]<10:
            dc[i]=10
    return


def pwm_init(motor):
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(motor[0][0], GPIO.OUT)
    GPIO.setup(motor[1][0], GPIO.OUT)
    GPIO.setup(motor[2][0], GPIO.OUT)
    GPIO.setup(motor[3][0], GPIO.OUT)

    GPIO.setup(motor[0][1], GPIO.OUT)
    GPIO.setup(motor[1][1], GPIO.OUT)
    GPIO.setup(motor[2][1], GPIO.OUT)
    GPIO.setup(motor[3][1], GPIO.OUT)

    GPIO.setup(motor[0][0], False)
    GPIO.setup(motor[1][0], False)
    GPIO.setup(motor[2][0], False)
    GPIO.setup(motor[3][0], False)

    GPIO.setup(motor[0][1], False)
    GPIO.setup(motor[1][1], False)
    GPIO.setup(motor[2][1], False)
    GPIO.setup(motor[3][1], False)


    return
def pwm_direction_control(pwm,direction,motor):
    for i in range(4):
        pwm[i].stop()
    for i in range(4):
        pwm[i]= GPIO.PWM(motor[i][direction[i]], 500)
        pwm[i].start(0)
    return
def pwm_dc_control(pwm,dc):
    for i in range(4):
        pwm[i].ChangeDutyCycle(dc[i])
    print(dc)
    return







