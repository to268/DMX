#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "receiver.h"
#include "common.h"

void* receiver_thread(void* arg);
void socket_init(struct shared_data* shared);
void socket_accept(int* server_fd, struct sockaddr_in* address, int* addrlen,
                    struct shared_data* shared);
void socket_handle(int client_fd, int server_fd, struct shared_data* shared);

inline void send_error(int client_fd);
inline void send_ack(int client_fd);

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
    int addrlen = sizeof(address);

    /* Creating socket file descriptor */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Socket fd creation failed\n", shared);
    }

    /* Populating socket informations */
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port        = htons(PORT);

    /* Binding socket to the target port */
    if ((bind(server_fd, (struct sockaddr*)&address, sizeof(address))) < 0) {
        error("Socket binding failed\n", shared);
    }

    /* Set listening mode */
    if ((listen(server_fd, 3)) < 0) {
        error("Socket listen failed\n", shared);
    }

    printf("Started Socket on port %d\n", PORT);

    socket_accept(&server_fd, &address, &addrlen, shared);

    return;
}

void socket_accept(int* server_fd, struct sockaddr_in* address, int* addrlen,
                    struct shared_data* shared)
{

    int client_fd;
    struct sockaddr_in client;
    int client_size = sizeof(client);

    while (1) {
        /* Accept incoming clients */
        if ((client_fd = accept(*server_fd, (struct sockaddr*)&client, (socklen_t*)&client_size)) < 0) {
            error("Socket accept failed\n", shared);
        }

        printf("New client (id %d)\n", client_fd);

        socket_handle(client_fd, *server_fd, shared);
    }
    close(*server_fd);
}

void socket_handle(int client_fd, int server_fd, struct shared_data* shared) {
    int read_idx;
    char tmp[BUFFER_SIZE];

    /* Initialize the temporary buffer */
    memset(tmp, 0, BUFFER_SIZE);

    /* Read incoming data */
    if ((read_idx = recv(client_fd, tmp, BUFFER_SIZE, 0)) < 0) {
        send_error(client_fd);
        error("data reception from the socket failed\n", shared);
    }

    printf("Received: %s", tmp);

    /* Handle stop server command */
    if (strstr(tmp, STOP_MSG)) {
        send_ack(client_fd);
        free_shared_data(shared);
        close(client_fd);
        close(server_fd);
        exit(EXIT_SUCCESS);
    }

    /* Acquire the lock to write on the shared memory */
    pthread_mutex_lock(&shared->lock);

    /* Write the received data on the shared memory */
    strncpy(shared->shared_buffer, tmp, BUFFER_SIZE);

    /* Release the lock to write on the shared memory */
    pthread_mutex_unlock(&shared->lock);

    printf("Copied received data to the shared memory\n");

    /* Set the state of the shared memory to modified */
    shared->state = STATE_MODIFIED;

    /* Send ACK */
    send_ack(client_fd);

    close(client_fd);
}

inline void send_ack(int client_fd) {
    send(client_fd, ACK_MSG, strlen(ACK_MSG), 0);
}

inline void send_error(int client_fd) {
    send(client_fd, ERROR_MSG, strlen(ERROR_MSG), 0);
}

void error(char* msg, struct shared_data* shared) {
    perror(msg);
    free_shared_data(shared);
    exit(EXIT_FAILURE);
}
