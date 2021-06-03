#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <string.h>

/* Change the slave address accordingly */
#define I2C_SLAVE_ADDRESS 0b0000100

#define I2C_MAX_NUMBER_LENGTH 4
#define I2C_MAX_DATA_LENGTH 7

struct i2c_data {
    uint8_t channel_high;
    uint8_t channel_low;
    uint8_t value;
};

void* gpio_thread(void* arg);

#endif /* GPIO_H */
