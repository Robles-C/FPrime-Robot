import RPi.GPIO as GPIO
import time
import math

#CONFIG
pinA = 17
pinB = 21

#Wheel specifications
WHEEL_DIAMETER_IN = 1.75
CPR = 26
CIRCUMFERENCE_IN = math.pi * WHEEL_DIAMETER_IN
DIST_PER_COUNT = CIRCUMFERENCE_IN / CPR

#State tracking
encoder_count = 0
last_state_a = 1

#gpio init
GPIO.setmode(GPIO.BCM)
GPIO.setup(pinA, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(pinB, GPIO.IN, pull_up_down=GPIO.PUD_UP)

print("Tracking encoder (GPIO17/21)... Press Ctrl+C to stop.")
try:
    while True:
        current_state_a = GPIO.input(pinA)

        if last_state_a == 1 and current_state_a == 0:
            b = GPIO.input(pinB)

            if b == GPIO.HIGH:
                encoder_count += 1
            else:
                encoder_count -= 1

            distance = encoder_count * DIST_PER_COUNT
            print(f"Count: {encoder_count} | Distance: {distance:.4f} in")

        last_state_a = current_state_a
        time.sleep(0.001)

except KeyboardInterrupt:
    print("\nExiting.")
    GPIO.cleanup()
