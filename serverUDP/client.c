#include "utils.h"
#include "utils.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  int UDP_SOCK;
  estados estadoAtual = SETUP_UDP;
  struct addrinfo *servAddrPtr;
  char *server;
  char *mat;

  for (;;)
  {
    switch (estadoAtual)
    {
    case SETUP_UDP:
      if (argc != 3) // Verifica se tem o número correto de argumentos
        DieWithUserMessage("Parameter(s)", "<Server Address/Name> ");
      server = argv[1];
      mat = argv[2];

      UDP_SOCK = prepararSocketUDPClient(server, &servAddrPtr);
      printf("Socket: %d\n", UDP_SOCK);
      printf("Familia: %d", servAddrPtr->ai_family);
      estadoAtual = MSG_HELLO;
      break;
    case MSG_HELLO:
      enviarHello(UDP_SOCK, mat);
      estadoAtual = FINAL;
      break;
    case FINAL:
      exit(EXIT_SUCCESS);
      break;
    default:
      exit(EXIT_FAILURE);
      break;
    }
  }
}