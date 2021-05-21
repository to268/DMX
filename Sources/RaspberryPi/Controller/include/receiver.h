#ifndef RECEIVER_H
#define RECEIVER_H

#define PORT 2369
#define MAX_CLIENTS_IN_QUEUE 3

void* receiver_thread(void* arg);

#endif /* RECEIVER_H */
