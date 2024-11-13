#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>

int initialize_server(uint16_t port);
void handle_server_events();
void cleanup_server();

#endif // SERVER_H
