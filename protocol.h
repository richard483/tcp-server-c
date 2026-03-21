#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdint.h>

typedef struct
{
  uint32_t length;
  uint8_t type;
  uint32_t correlation_id;
} Frame;

#define MSG_PING 0x01
#define MSG_PONG 0x02
#define MSG_ERROR_NOT_SUPPORTED 0xFF

int send_frame(int sockfd, Frame *frame);
int recv_frame(int sockfd, Frame *frame);

#endif