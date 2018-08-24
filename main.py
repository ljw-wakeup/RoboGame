import pwm
import RPi.GPIO as GPIO
import time


GPIO.setwarnings(False)
motor1 = (31, 32)
motor2 = (33, 36)
motor3 = (35, 38)
motor4 = (37, 40)
motor = (motor1, motor2, motor3, motor4)
direction = [0, 1, 0, 0]
dc = [50, 50, 0,0]
direction_old = [0, 1, 0, 0]
dc_old = [50, 50, 0, 0]

pwm.pwm_init(motor)
p1 = GPIO.PWM(motor[0][0], 500)
p2 = GPIO.PWM(motor[1][0], 500)
p3 = GPIO.PWM(motor[2][0], 500)
p4 = GPIO.PWM(motor[3][0], 500)
p = [p1, p2, p3, p4]
pwm.pwm_direction_control(p, direction, motor)
pwm.pwm_dc_control(p, dc)

try:
    while True:
        pass
        pwm.refreshmotor(dc,direction)
        if direction_old != direction:
             pwm.pwm_direction_control(p, direction, motor)
             direction_old = direction.copy()
        if dc_old != dc:
             pwm.pwm_dc_control(p, dc)
             dc_old = dc.copy()
finally:
    GPIO.cleanup()
