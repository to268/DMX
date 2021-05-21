#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "receiver.h"
#include "common.h"

void* receiver_thread(void* arg);
void socket_init(struct shared_data* shared);
void socket_handle(int* server_fd, struct sockaddr_in* address, int* addrlen,
                    struct shared_data* shared);
void error(char* msg, struct shared_data* shared);

void* receiver_thread(void* arg) {
    printf("Started receiver thread\n");

    /*
     * Cast passed pointer to a shared_data pointer struct
     * for initializing the socket
     */
    struct shared_data* shared = (struct shared_data*)arg;
    socket_init(shared);

    return NULL;
}

void socket_init(struct shared_data* shared) {
    /* Server side socket variables */
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    /* Creating socket file descriptor */
    if (!(server_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        error("Socket fd creation failed\n", shared);
    }

    /* Attaching socket to the target port */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        error("Socket attaching failed\n", shared);
    }

    /* Populating socket informations */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    /* Binding socket to the target port */
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        error("Socket binding failed\n", shared);
    }

    /* Set listening mode */
    if (listen(server_fd, MAX_CLIENTS_IN_QUEUE) < 0) {
        error("Socket listen failed\n", shared);
    }

    printf("Started Socket on port %d\n", PORT);

    socket_handle(&server_fd, &address, &addrlen, shared);

    return;
}

void socket_handle(int* server_fd, struct sockaddr_in* address, int* addrlen,
                    struct shared_data* shared)
{

    int sock;
    int readval;

    /* Accepting incoming clients */
    if ((sock = accept(*server_fd, (struct sockaddr*)address, (socklen_t*)addrlen) < 0)) {
        error("Socket accept failed\n", shared);
    }

    char tmp[BUFFER_SIZE];

    /* Read incoming data */
    /* FIX: The message needs to be received completely before continuing */
    readval = recv(sock, tmp, BUFFER_SIZE, 0);

    printf("Received data from a client, size %d\n", readval);

    /* Aquire the lock to write on the shared memory */
    pthread_mutex_lock(&shared->lock);

    /* Aquire the lock to write on the shared memory */
    strncpy(shared->shared_buffer, tmp, BUFFER_SIZE);

    /* Release the lock to write on the shared memory */
    pthread_mutex_unlock(&shared->lock);

    /* Set the state of the shared memory to modified */
    shared->state = STATE_MODIFIED;

    /* Flush temporary buffer */
    bzero(tmp, BUFFER_SIZE);

    /* Send ACK */
}

void error(char* msg, struct shared_data* shared) {
        perror(msg);
        free_shared_data(shared);
        exit(EXIT_FAILURE);
}
