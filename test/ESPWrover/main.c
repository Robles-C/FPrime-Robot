#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

// i2c config
#define I2C_SLAVE_NUM         I2C_NUM_0
#define I2C_SLAVE_ADDR        0x08
#define I2C_SLAVE_SDA_IO      GPIO_NUM_21
#define I2C_SLAVE_SCL_IO      GPIO_NUM_22
#define I2C_SLAVE_RX_BUF_LEN  128
#define I2C_SLAVE_TX_BUF_LEN  16

// motor config
#define LEFT_PWM_GPIO         GPIO_NUM_18
#define LEFT_DIR1_GPIO        GPIO_NUM_4
#define LEFT_DIR2_GPIO        GPIO_NUM_5

#define RIGHT_PWM_GPIO        GPIO_NUM_25
#define RIGHT_DIR1_GPIO       GPIO_NUM_33
#define RIGHT_DIR2_GPIO       GPIO_NUM_32

#define PWM_FREQ              1000
#define PWM_DUTY_RES          LEDC_TIMER_8_BIT

void i2c_slave_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_SLAVE,
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .slave = {
            .slave_addr = I2C_SLAVE_ADDR,
            .maximum_speed = 100000
        }
    };
    i2c_param_config(I2C_SLAVE_NUM, &conf);
    i2c_driver_install(I2C_SLAVE_NUM, I2C_MODE_SLAVE,
                       I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}

void pwm_init()
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = PWM_DUTY_RES,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = PWM_FREQ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t left_pwm = {
        .channel = LEDC_CHANNEL_0,
        .duty = 0,
        .gpio_num = LEFT_PWM_GPIO,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0,
        .hpoint = 0
    };
    ledc_channel_config(&left_pwm);

    ledc_channel_config_t right_pwm = {
        .channel = LEDC_CHANNEL_1,
        .duty = 0,
        .gpio_num = RIGHT_PWM_GPIO,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0,
        .hpoint = 0
    };
    ledc_channel_config(&right_pwm);
}

void set_motor_pwm(bool left, uint8_t duty)
{
    ledc_channel_t channel = left ? LEDC_CHANNEL_0 : LEDC_CHANNEL_1;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}

void motor_gpio_init()
{
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << LEFT_DIR1_GPIO) |
                        (1ULL << LEFT_DIR2_GPIO) |
                        (1ULL << RIGHT_DIR1_GPIO) |
                        (1ULL << RIGHT_DIR2_GPIO)
    };
    gpio_config(&io_conf);
}

void stop_motors()
{
    set_motor_pwm(true, 0);
    set_motor_pwm(false, 0);
}

void handle_motor_command(char dir, uint8_t speed)
{
    speed = (speed > 255) ? 255 : speed;

    switch (dir) {
        case 'F': //drive
            gpio_set_level(LEFT_DIR1_GPIO, 0);
            gpio_set_level(LEFT_DIR2_GPIO, 1);
            gpio_set_level(RIGHT_DIR1_GPIO, 0);
            gpio_set_level(RIGHT_DIR2_GPIO, 1);
            break;

        case 'B': // reverse
            gpio_set_level(LEFT_DIR1_GPIO, 1);
            gpio_set_level(LEFT_DIR2_GPIO, 0);
            gpio_set_level(RIGHT_DIR1_GPIO, 1);
            gpio_set_level(RIGHT_DIR2_GPIO, 0);
            break;

        case 'L': //left turn
            gpio_set_level(LEFT_DIR1_GPIO, 0);
            gpio_set_level(LEFT_DIR2_GPIO, 1);
            gpio_set_level(RIGHT_DIR1_GPIO, 1);
            gpio_set_level(RIGHT_DIR2_GPIO, 0);
            break;

        case 'R': // right turn
            gpio_set_level(LEFT_DIR1_GPIO, 1);
            gpio_set_level(LEFT_DIR2_GPIO, 0);
            gpio_set_level(RIGHT_DIR1_GPIO, 0);
            gpio_set_level(RIGHT_DIR2_GPIO, 1);
            break;

        case 'S': // stop
        default:
            stop_motors();
            return;
    }

    set_motor_pwm(true, speed);   //left
    set_motor_pwm(false, speed);  //right
}

//i2c task
void i2c_slave_task(void *arg)
{
    uint8_t data[2];
    while (1) {
        int len = i2c_slave_read_buffer(I2C_SLAVE_NUM, data, 2, 100 / portTICK_PERIOD_MS);
        if (len == 2) {
            char dir = data[0];
            uint8_t speed = data[1];
            handle_motor_command(dir, speed);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    i2c_slave_init();
    pwm_init();
    motor_gpio_init();

    xTaskCreate(i2c_slave_task, "i2c_slave_task", 2048, NULL, 10, NULL);
}