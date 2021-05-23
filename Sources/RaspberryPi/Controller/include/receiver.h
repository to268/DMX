#ifndef RECEIVER_H
#define RECEIVER_H

#define PORT 2369
#define MAX_CLIENTS_IN_QUEUE 3

#define ACK_MSG "OK\n"
#define ERROR_MSG "ERROR\n"
#define STOP_MSG "STOP"

void* receiver_thread(void* arg);

#endif /* RECEIVER_H */
