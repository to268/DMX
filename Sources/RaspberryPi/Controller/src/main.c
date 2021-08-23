/*
 * Copyright © 2021 to268 and contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the organization nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY yourname ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL yourname BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "receiver.h"
#include "common.h"
#include "gpio.h"

void handle_args(int argc, char **argv);
struct shared_data* initialize_shared_data(void);
void free_shared_data(struct shared_data* ptr);

/*  Launch process:
 *  Handle Arguments
 *  Launch Reciver Socket Thread
 *  Launch Gpio Thread
 */

int main(int argc, char **argv) {
    /* Handle Arguments */
    handle_args(argc, argv);

    /* Initialize shared_data struct */
    struct shared_data* shared = initialize_shared_data();

    /* Launch Gpio Socket Thread */
    pthread_t gpio;
    pthread_create(&gpio, NULL, gpio_thread, (void*)shared);

    /* Launch Reciver Socket Thread */
    pthread_t receiver;
    pthread_create(&receiver, NULL, receiver_thread, (void*)shared);

    /* Wait for Threads */
    pthread_join(receiver, NULL);
    pthread_join(gpio, NULL);

    free_shared_data(shared);

    return EXIT_SUCCESS;
}

void handle_args(int argc, char **argv) {
    if (argc < 2) {
        /* No argument is provided */
        return;
    }

    if (!strcmp(argv[1], "version")) {
#ifdef RELEASE
        printf("Release Version\n");
#else
        printf("Debug Version\n");
#endif
        exit(EXIT_SUCCESS);
    }

    printf("\n");
}

struct shared_data* initialize_shared_data(void) {
    /* Allocate the shared_data struct */
    struct shared_data* shared = malloc(sizeof(struct shared_data));

    /* Exit if malloc has failed */
    if (shared == NULL) {
        perror("Allocation of the shared_data struct failed\n");
        exit(EXIT_FAILURE);
    }

    /* Allocate the shared buffer */
    shared->shared_buffer = malloc(BUFFER_SIZE);

    /* Exit if malloc has failed */
    if (shared->shared_buffer == NULL) {
        perror("Allocation of the shared buffer failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Allocated %d bytes of shared memory\n", BUFFER_SIZE);

    /* Initialize mutex lock */
    pthread_mutex_init(&shared->lock, NULL);
    shared->state = 0;
    return shared;
}

void free_shared_data(struct shared_data* ptr) {
    /* Destroy mutex */
    pthread_mutex_destroy(&ptr->lock);
    /* Free shared buffer */
    free(ptr->shared_buffer);
    /* Free shared_data struct */
    free(ptr);
    printf("Freed the shared_data struct\n");
}
