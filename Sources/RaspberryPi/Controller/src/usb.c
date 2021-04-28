#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "usb.h"

void* usb_thread(void* arg) {
    printf("Started usb thread\n");
    return NULL;
}
