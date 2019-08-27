#include <stdio.h>
#include "Practical.h"
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc != 2) // Verifica o número correto de argumentos
    DieWithUserMessage("Parametro(s)", "<Porta Servidor/Servico>");

  char *service = argv[1]; // Primeiro arg:  porta local

  // Cria socket para conexoes
  int servSock = SetupTCPServerSocket(service);
  if (servSock < 0)
    DieWithUserMessage("SetupTCPServerSocket() falhou", service);

  for (;;) { // Laco infinito
    // Novas conexoes criam um novo socket conectado
    int clntSock = AcceptTCPConnection(servSock);

    HandleTCPClient(clntSock); // Manipula o cliente
    close(clntSock);
    break;
  }

  close(servSock);
}
