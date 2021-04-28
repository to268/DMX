#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "receiver.h"
#include "usb.h"

void handle_args(int argc, char **argv);

/*  Launch process:
 *  Handle Arguments
 *  Launch Reciver Socket Thread
 *  Launch Usb Thread
 */

int main(int argc, char **argv) {
    /* Handle Arguments */
    handle_args(argc, argv);

    /* Launch Reciver Socket Thread */
    pthread_t receiver;
    pthread_create(&receiver, NULL, receiver_thread, NULL);

    /* Launch Usb Thread */
    pthread_t usb;
    pthread_create(&usb, NULL, usb_thread, NULL);

    /* Wait for Threads */
    pthread_join(receiver, NULL);
    pthread_join(usb, NULL);

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
