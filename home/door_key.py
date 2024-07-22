import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

GPIO.setup(26, GPIO.IN)

key = GPIO.input(26)
print(key)

GPIO.cleanup()
