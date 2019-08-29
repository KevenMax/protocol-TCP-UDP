#include "utils.h"
#include "packets.h"
#include <netdb.h>
#include <string.h>
#include <stdio.h>

int prepararSocketUDPClient(char *server, struct addrinfo **servAddrPtr)
{
  // Diz ao sistema que tipos de endereços queremos
  struct addrinfo addrCriteria;                   // Critério para casamento de endereços
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Limpa a struct
  addrCriteria.ai_family = AF_UNSPEC;             // Qualquer família de endereços, aceita IPV4 ou IPV6
  // Para os campos a seguir, zero significa que não se importa
  addrCriteria.ai_socktype = SOCK_DGRAM;  // Somentes sockets datagrama
  addrCriteria.ai_protocol = IPPROTO_UDP; // Somente o protocolo UDP

  // Pega endereços
  struct addrinfo *servAddr; // Lista de endereços do servidor
  int rtnVal = getaddrinfo(server, UDP_PORT, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() falhou", gai_strerror(rtnVal));
  int sock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol); // Descritor de socket para o cliente
  if (sock < 0)
    DieWithSystemMessage("socket() falhou");

  *servAddrPtr = servAddr;
  printf("Familia: %d\n", servAddr->ai_family);
  return sock;
}

void enviarHello(int UDP_SOCK, char *mat, struct addrinfo *servAddr)
{
  Pacote *pkt = constroiPacote(HELLO_SIZE, PSECRET, 1, *mat, constroiPayload1());
  uint8_t outbuf[BUFSIZE];
  size_t reqSize = Encode(pkt, outbuf, BUFSIZE, MSG_HELLO);
  size_t numBytes = sendto(*sockPtr, outbuf, reqSize, 0, servAddr->ai_addr, servAddr->ai_addrlen);
}
