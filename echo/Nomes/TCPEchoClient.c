#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int main(int argc, char *argv[]) {

  if (argc < 3 || argc > 4) // Testa a quantidade certa de argumentos
    DieWithUserMessage("Parametro(s)",
        "<Endereco Servidor/Nome> <Palavra a ecoar> [<Porta Servidor/Servico>]");

  char *server = argv[1];     // Primeiro arg: endereco servidor/nome
  char *echoString = argv[2]; // Segundo arg: string a ecoar
  // Terceiro arg (opcional): porta servidor/servico
  char *service = (argc == 4) ? argv[3] : "echo";

  // Cria um socket TCP conectado
  int sock = SetupTCPClientSocket(server, service);
  if (sock < 0)
    DieWithUserMessage("SetupTCPClientSocket() falhou", "impossivel conectar");

  size_t echoStringLen = strlen(echoString); // Determina tamanho da entrada

  // Envia string ao servidor
  ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
  if (numBytes < 0)
    DieWithSystemMessage("send() falhou");
  else if (numBytes != echoStringLen)
    DieWithUserMessage("send()", "enviou um numero inesperado de bytes");

  // Recebe o eco do servidor
  unsigned int totalBytesRcvd = 0; // Conta a quantidade total de bytes recebidos
  fputs("Received: ", stdout);     // Prepara a impressao da string ecoada
  while (totalBytesRcvd < echoStringLen) {
    char buffer[BUFSIZE]; // buffer
    // Recebe ate o tamanho do buffer - 1, para deixar espaco para /0 
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    if (numBytes < 0)
      DieWithSystemMessage("recv() falhou");
    else if (numBytes == 0)
      DieWithUserMessage("recv()", "conexao fechada prematuramente");
    totalBytesRcvd += numBytes; // Mantem contagem dos bytes recebidos
    buffer[numBytes] = '\0';    // Completa a string
    fputs(buffer, stdout);      // Imprime o buffer
  }

  fputc('\n', stdout); // Imprime novalinha

  close(sock);
  exit(0);
}
