#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "utils.h"
#include <stdio.h>

int prepararSocketUDP()
{
  struct addrinfo addrCriteria;                   // Criterios para o endereco
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Limpa a estrutura
  addrCriteria.ai_family = AF_UNSPEC;             // Qualquer familia de enderecos
  addrCriteria.ai_flags = AI_PASSIVE;             // Aceita qualquer endreco/porta
  addrCriteria.ai_socktype = SOCK_DGRAM;          // Somente socket datagrama (UDP)
  addrCriteria.ai_protocol = IPPROTO_UDP;         // Somente porta UDP

  struct addrinfo *servAddr; // Lista de enderecos do servidor
  int rtnVal = getaddrinfo(NULL, UDP_PORT, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() falhou", gai_strerror(rtnVal));

  int sock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol);
  if (sock < 0)
    DieWithSystemMessage("socket() falhou");
  printf("Socket: %d\n", sock);

  return sock;
}