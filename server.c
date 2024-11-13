#include "server.h"
#include "common.h"
#include "messages.h"
#include <enet/enet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

static ENetHost *server = NULL;
static ENetEvent event;

void process_message(ENetEvent *event);

int initialize_server(uint16_t port) {
  if (enet_initialize() != 0) {
    fprintf(stderr, "Error al inicializar ENet.\n");
    return -1;
  }
  atexit(enet_deinitialize); // Limpia ENet al final del programa

  ENetAddress address;
  address.host = ENET_HOST_ANY; // Aceptar conexiones de cualquier host
  address.port = port;

  server = enet_host_create(&address, 32, 2, 0, 0);
  if (server == NULL) {
    fprintf(stderr, "No se pudo iniciar el servidor ENet.\n");
    return -1;
  }

  printf("Servidor iniciado en el puerto %u.\n", address.port);
  return 0;
}

void list_peers() {
  for (int i = 0; i < server->peerCount; i++) {
    ENetPeer *peer =
        &server->peers[i]; // Acceder al puntero del `peer` en la posición `i`

    // Comprobar si el `peer` está conectado
    if (peer->state != ENET_PEER_STATE_DISCONNECTED) {
      printf("Peer ID: %u, Dirección: %x:%u\n", peer->connectID,
             peer->address.host, peer->address.port);
    }
  }
}

void echo() {
  for (int i = 0; i < server->peerCount; i++) {
    ENetPeer *peer =
        &server->peers[i]; // Acceder al puntero del `peer` en la posición `i`

    // Comprobar si el `peer` está conectado
    if (peer->state != ENET_PEER_STATE_DISCONNECTED) {
      send_message(peer);
    }
  }
}

void handle_server_events() {
  while (1) { // Ejecutar indefinidamente
    while (enet_host_service(server, &event, 10) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        printf("Cliente conectado desde %x:%u.\n", event.peer->address.host,
               event.peer->address.port);
        echo();
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        // Aquí es donde procesamos el paquete recibido
        process_message(&event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        printf("Cliente desconectado.\n");
        event.peer->data = NULL;
        break;
      default:
        break;
      }
    }

    // list_peers();
  }
}

void cleanup_server() {
  if (server) {
    enet_host_destroy(server);
  }
}

void process_message(ENetEvent *event) {

  if (event->packet->dataLength == sizeof(SingleNumber)) {
    SingleNumber coords;
    memcpy(&coords, event->packet->data,
           sizeof(SingleNumber)); // Deserializar

    // Ahora tenemos acceso a las coordenadas
    printf("SingleNumber recibido - Number: %d\n", coords.n);
  } else {
    printf("Paquete recibido con datos no válidos\n");
  }
  enet_packet_destroy(event->packet);
  // Aquí es donde se imprime el mensaje recibido del cliente
  // printf("Mensaje recibido del cliente: %s\n", (char *)event->packet->data);
  // enet_packet_destroy(event->packet);
}
