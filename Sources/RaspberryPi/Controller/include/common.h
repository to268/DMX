#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <pthread.h>

#define BUFFER_SIZE 32

/* Constant values for the shared_data modified variable */
#define STATE_PROCESSED 0
#define STATE_MODIFIED  1

struct shared_data {
    char* shared_buffer;
    pthread_mutex_t lock;
    uint8_t state;
};

void free_shared_data(struct shared_data* ptr);

#endif /* COMMON_H */
