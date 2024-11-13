
#ifndef CLIENT_H
#define CLIENT_H

#include <enet/enet.h>

int initialize_client(const char *server_host, uint16_t server_port);
void handle_client_events();
void cleanup_client();

#endif // CLIENT_H
