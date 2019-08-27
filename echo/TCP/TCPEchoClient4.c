#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

// /.cliente ip_server palavra porta_serv

// ARGV
// [0] Cliente
// [1] Ip server
// [2] Palavra
// [3] Porta Serv

// int *pont; criando uma variável que vai apontar para um tipo inteiro
// int a = 5;
// int pont = &a; Guarda a referência para a memória a;
// int *pont++; incrementando o valor de a, no caso fica 5;
//

//  Segundo parametro, uma string
int main(int argc, char *argv[])
{

  if (argc < 3 || argc > 4) // Testa para verificar o número correto de argumentos
    DieWithUserMessage("Parametro(s)", "<Endereço servidor> <Palavra a ecoar> [<Porta servidor>]");

  // ponteiros para o array argv
  char *servIP = argv[1];     // Primeiro argumento: endereço servidor IP (formato legível, com pontos.)
  char *echoString = argv[2]; // Segundo argumento: string para ecoar

  // Terceiro argumento (opcional): porta servidor (numerica).  7 é uma porta bem conhecida para echo
  // in_port_t é um inteiro sem sinal de 16 bits(uint16_t da especificação POSIX) definido em netinet/in.h.
  in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;

  // Cria um socket de fluxo confiável utilizando TCP
  int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // o parâmetro protocolo, o terceiro, poderia ser 0. TCP seria escolhido por ser o padrão.
  // valores menores que 0 significa que o sistema operacional não conseguiu criar o canal de conexão
  if (sock < 0)
    DieWithSystemMessage("socket() falhou");

  // Constrói a struct(16 bytes) de endereço do servidor (IP e porta)
  struct sockaddr_in servAddr;            // Endereço servidor
  memset(&servAddr, 0, sizeof(servAddr)); // Limpa a struct, para não ter lixo na memória
  servAddr.sin_family = AF_INET;          // Família de endereços IPv4
  // Converte o endereço do formato de ponto em binário e armazena em servAddr.sin_addr.s_addr
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() falhou", "string de endereço inválida");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() falhou");
  servAddr.sin_port = htons(servPort); // Porta Servidor, convertida do formato host para rede. a função htons converte dois tipos: short e long.

  // Estabelece conexão com servidor echo, do host(sock) com o server(servAddr);
  if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() falhou");

  size_t echoStringLen = strlen(echoString); // Determina tamanho da entrada

  // Envia string ao servidor através do send o host, a palavra, o tamanho da palavra e o comando 0 para identificar o tipo de operação.
  ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
  if (numBytes < 0)
    DieWithSystemMessage("send() falhou");
  // Verificar se o número de bytes foi enviado corretamente.
  else if (numBytes != echoStringLen)
    DieWithUserMessage("send()", "enviou um número inesperado de bytes");

  // Recebe a mesma string de volta do servidor
  unsigned int totalBytesRcvd = 0; // Conta o total de bytes recebidos
  fputs("Recebido: ", stdout);     // Prepara para imprimir a string ecoada
  while (totalBytesRcvd < echoStringLen)
  {
    char buffer[BUFSIZE]; // E/S buffer
    /* Recebeu até o tamanho do buffer, menos 1 para deixar o espaço para
    o null, em bytes do emissor */
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    if (numBytes < 0)
      DieWithSystemMessage("recv() falhou");
    else if (numBytes == 0)
      DieWithUserMessage("recv()", "conexão fechada prematuramente");
    totalBytesRcvd += numBytes; // Mantém a contagem do total de bytes
    buffer[numBytes] = '\0';    // Termina a string!
    fputs(buffer, stdout);      // Imprime o eco do buffer
  }

  fputc('\n', stdout); // Imprime uma linha vazia

  close(sock);
  exit(0);
}
// gcc -o servidor TCPEchoServer4.c DieWithMessage.c AddressUtility.c TCPServerUtility.c