#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "utils.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  int sockUDP;
  estados estadoAtual = SETUP_UDP;
  for (;;)
  {
    switch (estadoAtual)
    {
    case SETUP_UDP:
      sockUDP = prepararSocketUDP();
      estadoAtual = MSG_HELLO;
      break;
    case MSG_HELLO:
      printf("Vou receber Hello!\n");
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