from mpu6050 import mpu6050
import time

sensor = mpu6050(0x68)

#Calibration
print("Calibrating gyro bias... Keep sensor still...")
samples = 500
z_bias = 0.0

for _ in range(samples):
    gyro = sensor.get_gyro_data()
    z_bias += gyro['z']
    time.sleep(0.005)

z_bias /= samples
print(f"Calibrated Z bias: {z_bias:.4f}°/s")

#Heading Tracker
heading = 0.0
last_time = time.time()

try:
    while True:
        now = time.time()
        dt = now - last_time
        last_time = now

        gyro_data = sensor.get_gyro_data()
        z_rate = gyro_data['z'] - z_bias

        heading += z_rate * dt
        heading %= 360.0 

        print(f"Heading: {heading:.2f}°")
        time.sleep(0.05)

except KeyboardInterrupt:
    print("\nStopped.")
