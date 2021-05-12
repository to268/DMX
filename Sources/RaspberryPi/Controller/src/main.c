#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "receiver.h"

void handle_args(int argc, char **argv);

/*  Launch process:
 *  Handle Arguments
 *  Launch Reciver Socket Thread
 *  Launch Gpio Thread
 */

int main(int argc, char **argv) {
    /* Handle Arguments */
    handle_args(argc, argv);

    /* Launch Reciver Socket Thread */
    pthread_t receiver;
    pthread_create(&receiver, NULL, receiver_thread, NULL);

    /* Wait for Threads */
    pthread_join(receiver, NULL);

    return EXIT_SUCCESS;
}

void handle_args(int argc, char **argv) {
    if (argc < 2) {
        /* No argument is provided */
        return;
    }

    /* Print arguments for now */
    printf("Provided Arguments: ");
    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }

    printf("\n");
}
