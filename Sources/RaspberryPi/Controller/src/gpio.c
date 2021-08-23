#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <pigpio.h>
#include "common.h"
#include "gpio.h"

void* gpio_thread(void* arg);
void gpio_acquire(struct shared_data* shared);
int gpio_send(char* buffer);
int i2c_convert(char* buffer, struct i2c_data* data);
int i2c_send(unsigned int handle, struct i2c_data* data);

void* gpio_thread(void* arg) {
    printf("Started gpio thread\n");

    /* Cast passed pointer to a shared_data pointer struct */
    struct shared_data* shared = (struct shared_data*)arg;

    while (1) {
        /* Check if the shared buffer has been modified since we processed it */
        if (shared->state == STATE_MODIFIED) {
            /* Aquire the data on the shared memory */
            gpio_acquire(shared);
        }
    }

    return NULL;
}

void gpio_acquire(struct shared_data* shared) {
    char tmp[BUFFER_SIZE];

    /* Acquire the lock to read the shared memory */
    pthread_mutex_lock(&shared->lock);

    /* Read the data on the shared memory */
    strncpy(tmp, shared->shared_buffer, BUFFER_SIZE);

    /* Release the lock to read the shared memory */
    pthread_mutex_unlock(&shared->lock);

    /* Send the data thought gpio pins using I2C protocol */
    if (!gpio_send(tmp)) {
        free_shared_data(shared);
        exit(EXIT_FAILURE);
    }

    /* Set the state of the shared memory to processed */
    shared->state = STATE_PROCESSED;
}


int gpio_send(char* buffer) {
    unsigned int handle;

    /* Convert the data to send it */
    struct i2c_data data;
    if (!i2c_convert(buffer, &data)) {
        perror("I2C data coversion failed");
        return 0;
    }

    printf("Channel higher half: %d\n", data.channel_high);
    printf("Channel lower half: %d\n", data.channel_low);
    printf("Channel value: %d\n", data.value);

    /* Initialize the gpio library */
    gpioInitialise();

    /* Open an I2C connexion with SDA: gpio 2 (pin 3), SCL: gpio 3 (pin 5) */
    if ((handle = i2cOpen(1, I2C_SLAVE_ADDRESS, 0)) < 0) {
        perror("GPIO open I2C connexion failed\n");
        return 0;
    }

    if (!i2c_send(handle, &data)) {
        perror("I2C send data failed");
        return 0;
    }

    i2cClose(handle);

    /* Terminate the gpio library */
    gpioTerminate();

    return 1;
}

int i2c_convert(char* buffer, struct i2c_data* data) {
    uint16_t channel;
    uint8_t value;

    /* Store values as a string to convert the data */
    char channel_str[I2C_MAX_NUMBER_LENGTH];
    char value_str[I2C_MAX_NUMBER_LENGTH];

    /* Copy the first value */
    char* idx = strstr(buffer, " ");
    if (idx <= 0)
        return 0;
    int length = idx - buffer;
    strncpy(channel_str, buffer, length);

    /* Copy the second value */
    idx++;
    char* end = strstr(buffer, "\n");
    if (end <= 0)
        end = (buffer + strlen(buffer));
    strncpy(value_str, idx, (end - idx));

    /* Convert a string to a int */
    channel = atoi(channel_str);
    value = atoi(value_str);

    /*
     * Split the channel variable higher and lower half
     * Store values in the struct
     */
    data->channel_high  = (channel & 0xff00) >> 8;
    data->channel_low   = (channel & 0x00ff);
    data->value         = value;

    return 1;
}

int i2c_send(unsigned int handle, struct i2c_data* data) {
    if (i2cWriteByte(handle, data->channel_high) != 0) {
        return 0;
    }

    if (i2cWriteByte(handle, data->channel_low) != 0) {
        return 0;
    }

    if (i2cWriteByte(handle, data->value) != 0) {
        return 0;
    }

    printf("The data has been sent thought the gpio\n");
    return 1;
}
