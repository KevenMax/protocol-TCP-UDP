#ifndef UTILS_H_
#define UTILS_H_

#define UDP_PORT "12235"
#define PAYLOAD_LEN_HELLO 12
// #define PSECRET 10
#define STEP_CLIENT 1
#define STEP_SERVER 2
// #define HELLO_MSG "hello world"
#define HDR_LEN 12
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

// Manipula erro com mensagem do usuário
void DieWithUserMessage(const char *msg, const char *detail);
// Manipula erro com mensagem do sistema
void DieWithSystemMessage(const char *msg);
// Preparar socket UDP servidor
int prepararSocketUDP(void);
// Preparar socket UDP cliente
int prepararSocketUDPClient(char *server, struct addrinfo **servAddr);
// Enviar pacote hello
void enviarHello(int UDP_SOCK, char *mat, struct addrinfo *servAddr);

typedef enum
{
  SETUP_UDP,
  MSG_HELLO,
  MSG_HELLO_RESPONSE,
  FINAL
} estados;

#endif
