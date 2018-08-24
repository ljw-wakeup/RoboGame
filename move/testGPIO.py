import RPi.GPIO as GPIO
import time
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(12, GPIO.OUT, initial = GPIO.HIGH)
try:
	while True:
		time.sleep(20)
finally: 
	GPIO.cleanup()


