#!/usr/bin/env python3
import time
from adafruit_servokit import ServoKit

# init
kit = ServoKit(channels=16)

servo7 = kit.servo[7]
servo12 = kit.servo[12]

servo7.set_pulse_width_range(550, 2200)
servo12.set_pulse_width_range(620, 1750)

#servo7_pwms = [550, 1200, 2200]
#servo12_pwms = [620, 1575, 1750]
servo7_pwms = [1200]
servo12_pwms = [1750]

servo_delay = 1.0

def pwm_to_angle(pwm_us, min_us, max_us):
    """Convert pulse width to angle in degrees."""
    return (pwm_us - min_us) * 180.0 / (max_us - min_us)

def move_servos():
    for i in range(len(servo7_pwms)):
        pwm7 = servo7_pwms[i]
        angle7 = max(0, min(180, pwm_to_angle(pwm7, 550, 2200)))
        print(f"Moving Channel 7 → {pwm7} µs ({angle7:.1f}°)")
        servo7.angle = angle7
        time.sleep(servo_delay)

        pwm12 = servo12_pwms[i]
        angle12 = max(0, min(180, pwm_to_angle(pwm12, 620, 1750)))
        print(f"Moving Channel 12 → {pwm12} µs ({angle12:.1f}°)")
        servo12.angle = angle12
        time.sleep(servo_delay)

if __name__ == "__main__":
    try:
        while True:
            move_servos()
    except KeyboardInterrupt:
        print("\nStopped by user.")

