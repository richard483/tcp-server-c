#include "protocol.h"
#include <unistd.h>
#include <arpa/inet.h>

static void pack_uint32(uint8_t *buf, uint32_t value)
{
  buf[0] = (value >> 24) & 0xFF;
  buf[1] = (value >> 16) & 0xFF;
  buf[2] = (value >> 8) & 0xFF;
  buf[3] = (value) & 0xFF;
}

int send_frame(int sockfd, Frame *frame)
{
  uint8_t buf[9];
  int write_result;

  pack_uint32(buf, frame->length);
  buf[4] = frame->type;
  pack_uint32(buf + 5, frame->correlation_id);

  write_result = write(sockfd, buf, 9);

  if (write_result == -1)
    return -1;

  return 0;
}