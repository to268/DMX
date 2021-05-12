#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "receiver.h"

void* receiver_thread(void* arg);

void* receiver_thread(void* arg) {
    printf("Started receiver thread\n");
    return NULL;
}
