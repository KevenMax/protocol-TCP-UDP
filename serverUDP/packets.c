#include "packets.h"
#include "utils.h"

Payload_1 *constroiPayload1(void)
{
  Payload_1 *payload = malloc(sizeof(Payload_1));
  payload->msg = malloc(HELLO_SIZE);
  // copiar para o 1 argumento, o valor do 2 argumento com a quantidade de bytes do 3 argumento.
  memcpy(payload->msg, HELLO_MSG, HELLO_SIZE);
  return payload;
}

Pacote *constroiPacote(uint32_t payload_len, uint32_t psecret, uint16_t step, uint16_t student_id, void *payload)
{
  Pacote *pkt = malloc(sizeof(Pacote));
  pkt->payload_len = payload_len;
  pkt->psecret = psecret;
  pkt->step = step;
  pkt->student_id = student_id;
  pkt->payload = payload;

  return pkt;
}

size_t Encode(Pacote *v, uint8_t *outBuf, size_t bufSize, estados state)
{
  size_t size = 0;
  uint16_t u16 = 0;
  uint32_t u32 = 0;

  // Verificar se tem espaços no buffer para armazenar o pacote
  if (bufSize < sizeof(Pacote))
  {
    DieWithUserMessage("Buffer muito pequeno", "");
  }

  // Limpar o buffer para ter certeza
  memset(outBuf, 0, bufSize);

  // Conversão de cabeçalho comum.
  // Converte o payload_len e coloca no buffer
  u32 = htonl(pkt->payload_len);
  memcpy(outBuf + PAYLOAD_LEN_SHIFT, &u32, UI32SIZE);
  size += UI32SIZE;

  u32 = htonl(pkt->psecret);
  memcpy(outBuf + PSECRET_SHIFT, &u32, UI32SIZE);
  size += UI32SIZE;
  // converte  step e coloca no buffer
  u16 = htons(pkt->student_id);
  memcpy(outBuf + STUDENT_ID_SHIFT, &u16, UI16SIZE);
  size += UI16SIZE;

  switch (state)
  {
  case MSG_HELLO:
    size += encodeHello(pkt, outBuf, size);
    break;
  case MSG_HELLO_RESPONSE:
    // size += encodeHelloResp(pkt, outBuf, size);
    break;
  default:
    break;
  }

  return size;
}

size_t encodeHello(Pacote *pkt, uint8_t *outBuf, size_t size)
{
  memcpy(outBuf + size, ((Payload_1 *)pkt->payload)->msg, HELLO_SIZE);
  size += HELLO_SIZE;
  return size;
}