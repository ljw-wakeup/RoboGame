import pwm
import RPi.GPIO as GPIO
import pid

motor1 = (7, 11)
motor2 = (13, 15)
motor3 = (29, 31)
motor4 = (33, 35)
motor = (motor1, motor2, motor3, motor4)
direction = [0, 0, 0, 0]
dc = [50, 50, 50, 50]
direction_old = [0, 0, 0, 0]
dc_old= [50, 50, 50, 50]

GPIO.cleanup()
pwm.pwm_init(motor)
p1 = GPIO.PWM(motor[0][0], 50)
p2 = GPIO.PWM(motor[1][0], 50)
p3 = GPIO.PWM(motor[2][0], 50)
p4 = GPIO.PWM(motor[3][0], 50)
p = [p1, p2, p3, p4]

pwm.pwm_direction_control(p,direction,motor)
try:
    while True:
        pid.refreshmotor(direction,dc)
        if direction_old!=direction:
            pwm.pwm_direction_control()
            direction_old=direction.copy()
        if dc_old!=dc:
            pwm.pwm_dc_control()
            dc_old=dc.copy()
finally:
    GPIO.cleanup()
