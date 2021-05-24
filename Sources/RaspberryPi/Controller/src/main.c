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
