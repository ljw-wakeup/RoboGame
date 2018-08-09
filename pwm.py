import RPi.GPIO as GPIO
import time
import pid

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
        pwm[i]= GPIO.PWM(motor[i][direction[i]], 50)
        pwm[i].start(0)
    return
def pwm_dc_control(pwm,dc):
    for i in range(4):
        pwm[i].ChangeDutyCycle(dc[i])
    return






