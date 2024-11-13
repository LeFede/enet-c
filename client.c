#include "client.h"
#include "common.h"
#include "messages.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ENetHost *client = NULL;
static ENetPeer *peer = NULL;
static ENetEvent event;

int initialize_client(const char *server_host, uint16_t server_port) {
  if (enet_initialize() != 0) {
    fprintf(stderr, "Error al inicializar ENet.\n");
    return -1;
  }
  atexit(enet_deinitialize); // Limpia ENet al final del programa

  client = enet_host_create(NULL, 1, 2, 0, 0);
  if (client == NULL) {
    fprintf(stderr, "No se pudo crear el cliente.\n");
    return -1;
  }

  ENetAddress address;
  enet_address_set_host(&address, server_host); // Dirección del servidor
  address.port = server_port;

  peer = enet_host_connect(client, &address, 2, 0);
  if (peer == NULL) {
    fprintf(stderr, "No se pudo conectar al servidor.\n");
    return -1;
  }

  if (enet_host_service(client, &event, 5000) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT) {
    printf("Conectado al servidor.\n");
    return 0;
  } else {
    fprintf(stderr, "No se pudo establecer la conexión con el servidor.\n");
    enet_peer_reset(peer);
    enet_host_destroy(client);
    return -1;
  }

  // send_message(peer);
  // handle_events(client, event, peer);

  // return 0;
}

void handle_client_events() {
  while (1) { // Ejecutar indefinidamente
    while (enet_host_service(client, &event, 100) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_RECEIVE:
        printf("llego un mensaje\n");
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

// void handle_events() {
//   while (1) { // Mantener el cliente funcionando
//     while (enet_host_service(client, &event, 10) > 0) {
//       switch (event.type) {
//       case ENET_EVENT_TYPE_RECEIVE:
//         printf("Mensaje recibido del servidor: %s\n",
//                (char *)event.packet->data);
//         enet_packet_destroy(event.packet);
//         break;
//       case ENET_EVENT_TYPE_DISCONNECT:
//         printf("Desconectado del servidor.\n");
//         enet_host_destroy(client);
//         return;
//       default:
//         break;
//       }
//     }
//
//     // Esperar el input del usuario y enviar un mensaje
//     char message[256];
//     printf(
//         "Escribe un mensaje para enviar al servidor (o 'exit' para salir):
//         ");
//     fgets(message, sizeof(message), stdin);
//
//     // Remover el salto de línea al final del mensaje
//     message[strcspn(message, "\n")] = 0;
//
//     if (strcmp(message, "exit") == 0) {
//       printf("Cerrando cliente.\n");
//       break;
//     }
//
//     send_message(peer); // Enviar el mensaje al servidor
//     // send_message(message); // Enviar el mensaje al servidor
//   }
// }

void cleanup_client() {
  if (client) {
    enet_host_destroy(client);
  }
  if (peer) {
    enet_peer_reset(peer);
  }
}
