#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int main(int argc, char *argv[])
{

  if (argc != 2) // Verifica se o numero de argumentos esta correto
    DieWithUserMessage("Parameter(s)", "<Server Port/Service>");

  char *service = argv[1]; // Primeiro arg: porta/servico local

  // Constroi a estrutura de endereço do servidor
  struct addrinfo addrCriteria;                   // Criterios para o endereco
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Limpa a estrutura
  addrCriteria.ai_family = AF_UNSPEC;             // Qualquer familia de enderecos
  addrCriteria.ai_flags = AI_PASSIVE;             // Aceita qualquer endreco/porta
  addrCriteria.ai_socktype = SOCK_DGRAM;          // Somente socket datagrama
  addrCriteria.ai_protocol = IPPROTO_UDP;         // Somente porta UDP

  struct addrinfo *servAddr; // Lista de enderecos do servidor
  int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() falhou", gai_strerror(rtnVal));

  // Cria socket para aceitar conexoes
  int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
                    servAddr->ai_protocol);
  if (sock < 0)
    DieWithSystemMessage("socket() falhou");

  // Se liga a um endereco local
  if (bind(sock, servAddr->ai_addr, servAddr->ai_addrlen) < 0)
    DieWithSystemMessage("bind() falhou");

  // Libera a lista de enderecos retornada pelo getaddrinfo()
  freeaddrinfo(servAddr);

  for (;;)
  {                                   // Laco infinito
    struct sockaddr_storage clntAddr; // Endereco cliente
    // Define o tamanho da struct baseado na struct endereco
    socklen_t clntAddrLen = sizeof(clntAddr);

    // Bloqueia ate receber uma mensagem do cliente
    char buffer[MAXSTRINGLENGTH]; // buffer
    // Tamanho da mensagem recebida
    ssize_t numBytesRcvd = recvfrom(sock, buffer, MAXSTRINGLENGTH, 0, (struct sockaddr *)&clntAddr, &clntAddrLen);
    if (numBytesRcvd < 0)
      DieWithSystemMessage("recvfrom() falhou");

    fputs("Manipulando cliente ", stdout);
    PrintSocketAddress((struct sockaddr *)&clntAddr, stdout);
    fputc('\n', stdout);
    fputs(buffer, stdout);
    fputc('\n', stdout);
  }
}
