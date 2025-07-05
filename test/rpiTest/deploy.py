#!/usr/bin/env python3
import time
from adafruit_servokit import ServoKit
import smbus

# setup
kit = ServoKit(channels=16)

# servos used
servo_arm = kit.servo[12]    # Arm servo on channel 12
servo_drive = kit.servo[7]   # Drive servo on channel 7

# custom ranges based on tests
# big servo 620, 1.5k, 1.75k
servo_arm.set_pulse_width_range(620, 1750)
servo_drive.set_pulse_width_range(550, 2200)

# wrover setup
I2C_ADDR = 0x08
bus = smbus.SMBus(1)

arm_up_us = 1750
arm_down_us = 620
drive_pwms = [550, 1200, 2200]

def pwm_to_angle(pwm_us, min_us, max_us):
    """Convert PWM pulse width to angle (0–180)"""
    return (pwm_us - min_us) * 180.0 / (max_us - min_us)

def set_servo_pwm(servo, pwm_us, min_us, max_us):
    angle = max(0, min(180, pwm_to_angle(pwm_us, min_us, max_us)))
    servo.angle = angle

def send_i2c_command(command_bytes):
    try:
        bus.write_i2c_block_data(I2C_ADDR, command_bytes[0], command_bytes[1:])
        print(f"I2C Sent: {[hex(b) for b in command_bytes]}")
    except Exception as e:
        print(f"I2C Error: {e}")

def run_sequence():
    print("Starting sequence...\n")
    time.sleep(10)
    print("Arm: moving to UP (1750 µs)")
    set_servo_pwm(servo_arm, arm_up_us, 620, 1750)
    time.sleep(1)

    print("Arm: slowly lowering to 620 µs")
    steps = 26
    for i in range(steps + 1):
        pwm = arm_up_us - (i * (arm_up_us - arm_down_us) / steps)
        set_servo_pwm(servo_arm, pwm, 620, 1750)
        time.sleep(0.05)

    print("I2C: Sending 0x46 0xEE")
    send_i2c_command([0x46, 0xEE])
    time.sleep(1)

    print("I2C: Sending 0x46 0x00")
    send_i2c_command([0x46, 0x00])
    time.sleep(1)

    print("Arm: returning to UP (1750 µs)")
    set_servo_pwm(servo_arm, arm_up_us, 620, 1750)
    time.sleep(1)

    print("\nSweeping drive servo (channel 7):")
    for pwm in drive_pwms:
        set_servo_pwm(servo_drive, pwm, 550, 2200)
        print(f"Drive PWM: {pwm}")
        time.sleep(1)

    print("\nSequence complete.")

if __name__ == "__main__":
    try:
        run_sequence()
    except KeyboardInterrupt:
        print("\nStopped by user.")
