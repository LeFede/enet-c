#ifndef COMMON_H
#define COMMON_H

#include <enet/enet.h>
void send_message(ENetPeer *peer);
void handle_events(ENetHost *host, ENetEvent event, ENetPeer *peer);

#endif
