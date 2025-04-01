import lgpio
import time

# GPIO setup
CHIP = 0
TRIG = 23  #Trig
ECHO = 24  #Echo
LED = 13   #Indicator LED

h = lgpio.gpiochip_open(CHIP)
lgpio.gpio_claim_output(h, TRIG)
lgpio.gpio_claim_input(h, ECHO)
lgpio.gpio_claim_output(h, LED)

def get_distance():
    #trigger pulse
    lgpio.gpio_write(h, TRIG, 1)
    time.sleep(0.00001)
    lgpio.gpio_write(h, TRIG, 0)

    #wait for echo to go high
    timeout = time.time() + 1
    while lgpio.gpio_read(h, ECHO) == 0:
        pulse_start = time.time()
        if time.time() > timeout:
            return -1

    #wait for echo to go low
    timeout = time.time() + 1
    while lgpio.gpio_read(h, ECHO) == 1:
        pulse_end = time.time()
        if time.time() > timeout:
            return -1

    pulse_duration = pulse_end - pulse_start
    distance = (pulse_duration * 34300) / 2
    return round(distance, 2)

try:
    while True:
        dist = get_distance()
        print(f"Distance: {dist} cm")

        if dist > 0 and dist < 15:
            lgpio.gpio_write(h, LED, 1)
            time.sleep(0.1)
            lgpio.gpio_write(h, LED, 0)
            time.sleep(0.1)
        else:
            lgpio.gpio_write(h, LED, 0)
            time.sleep(0.5)

except KeyboardInterrupt:
    pass
finally:
    lgpio.gpiochip_close(h)
