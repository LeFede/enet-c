#include "messages.h"
#include <enet/enet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void send_message(ENetPeer *peer) {
  ENetPacket *packet =
      enet_packet_create(NULL, sizeof(SingleNumber), ENET_PACKET_FLAG_RELIABLE);

  SingleNumber msg = {100};
  memcpy(packet->data, &msg, sizeof(SingleNumber));

  enet_peer_send(peer, 0, packet);
}

void handle_events(ENetHost *host, ENetEvent event, ENetPeer *peer) {
  while (1) { // Ejecutar indefinidamente
    while (enet_host_service(host, &event, 100) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        printf("Cliente conectado desde %x:%u.\n", event.peer->address.host,
               event.peer->address.port);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        printf("llego un mensaje");
        // Aquí es donde procesamos el paquete recibido
        // process_message(&event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        printf("Cliente desconectado.\n");
        event.peer->data = NULL;
        break;
      default:
        break;
      }
    }

    send_message(peer);

    // list_peers();
  }
}
