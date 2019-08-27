#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "Practical.h"

static const int MAXPENDING = 5; // Numero maximo de conexoes pendentes

int SetupTCPServerSocket(const char *service) {
  // Constroi a struct de enderecos do servidor
  struct addrinfo addrCriteria;                   // Criterio de enderecos
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Limpa struct
  addrCriteria.ai_family = AF_UNSPEC;             // Qualquer familia de enderecos
  addrCriteria.ai_flags = AI_PASSIVE;             // Aceita qualquer endereco/porta
  addrCriteria.ai_socktype = SOCK_STREAM;         // Somente sockets de fluxo
  addrCriteria.ai_protocol = IPPROTO_TCP;         // Somente protocolo TCP

  struct addrinfo *servAddr; // Lista de enderecos do servidor
  int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() falhou", gai_strerror(rtnVal));

  int servSock = -1;
  for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
    // Cria socket TCP
    servSock = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    if (servSock < 0)
      continue;       // Criacao do socket falhou, tenta um proximo

    // Associa um endereco local ao socket
    if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
        (listen(servSock, MAXPENDING) == 0)) {
      // Imprime o endereco local do socket
      struct sockaddr_storage localAddr;
      socklen_t addrSize = sizeof(localAddr);
      if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
        DieWithSystemMessage("getsockname() falhou");
      fputs("Associado a ", stdout);
      PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
      fputc('\n', stdout);
      break;       // Bind e listen com sucesso
    }

    close(servSock);  // Fecha e tenta novamente
    servSock = -1;
  }

  // Libera a lista de enderecos
  freeaddrinfo(servAddr);

  return servSock;
}

int AcceptTCPConnection(int servSock) {
  struct sockaddr_storage clntAddr; // Endereco cliente
  // Define o tamanho da struct de endereco
  socklen_t clntAddrLen = sizeof(clntAddr);

  // Espera um cliente conectar-se
  int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
  if (clntSock < 0)
    DieWithSystemMessage("accept() falhou");

  // clntSock esta conectado ao cliente

  fputs("Servido cliente ", stdout);
  PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
  fputc('\n', stdout);

  return clntSock;
}

void HandleTCPClient(int clntSocket) {
  char buffer[BUFSIZE]; // Buffer p/ eco

  // Recebe mensagem do cliente
  ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
    DieWithSystemMessage("recv() falhou");

  // Envia mensagem e espera por mais
  while (numBytesRcvd > 0) { // 0 indica fim do fluxo de informacao
    // Eco p/ o cliente
    ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
    if (numBytesSent < 0)
      DieWithSystemMessage("send() falhou");
    else if (numBytesSent != numBytesRcvd)
      DieWithUserMessage("send()", "enviou numero inesperado de bytes");

    // Verifica se tem mais bytes para receber
    numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
    if (numBytesRcvd < 0)
      DieWithSystemMessage("recv() falhou");
  }

  close(clntSocket); // Fecha socket cliente
}
