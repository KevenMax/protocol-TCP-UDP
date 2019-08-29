#ifndef _PACKETS_H_
#define _PACKETS_H_

#include <inttypes.h>

#define HELLO_MSG "Hello World"
#define HELLO_SIZE 12
#define PSECRET 43
#define UI32SIZE 4
#define UI16SIZE 2
#define BUFSIZE 512

enum
{
  PAYLOAD_LEN_SHIFT = 0,
  PSECRET_SHIFT = 4,
  STEP_SHIFT = 8,
  STUDENT_ID_SHIFT = 10,
  TAM_CABECALHO = 12,
  TAM_PAYLOAD_1 = 12,
  NUM_SHIFT = 12,
  LEN_SHIFT = 16,
  TCP_PORT_SHIFT = 20,
  SECRET_A_SHIFT = 24
};

struct packet
{
  uint32_t payload_len;
  uint32_t psecret;
  uint16_t step;
  uint16_t student_id;
  void *payload;
};

struct payload_1
{
  char *msg;
};

typedef struct packet Pacote;
typedef struct payload_1 Payload_1;

Pacote *constroiPacote(uint32_t payload_len, uint32_t psecret, uint16_t step, uint16_t student_id, void *payload);

Payload_1 *constroiPayload1(void);
size_t Encode(Pacote *v, uint8_t *outBuf, size_t bufSize, estados state);
size_t encodeHello(Pacote *pkt, uint8_t *outBuf, size_t size);
#endif