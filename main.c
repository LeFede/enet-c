#include "client.h" // Para las funciones del cliente
#include "server.h" // Para las funciones del servidor
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int opcion;

  // Mostrar men� de opciones
  printf("Seleccione una opci�n:\n");
  printf("1. Iniciar servidor\n");
  printf("2. Iniciar cliente\n");
  printf("Ingrese su opci�n: ");
  scanf("%d", &opcion); // Leer la opci�n del usuario

  switch (opcion) {
  case 1:
    printf("Iniciando servidor...\n");
    if (initialize_server(1234) == 0) {
      handle_server_events(); // Manejar los eventos del servidor
    } else {
      fprintf(stderr, "No se pudo inicializar el servidor.\n");
      return EXIT_FAILURE;
    }
    break;

  case 2:
    printf("Iniciando cliente...\n");
    if (initialize_client("localhost", 1234) == 0) {
      handle_client_events();
    } else {
      fprintf(stderr, "No se pudo inicializar el cliente.\n");
      return EXIT_FAILURE;
    }
    break;

  default:
    printf("Opci�n no v�lida.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
