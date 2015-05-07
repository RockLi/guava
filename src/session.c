#include "session.h"

#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <assert.h>

int guava_session_new_id(char *buf, uint32_t len) {
  int fd;
  int i;
  char b[8] = {0};

  if (len < GUAVA_SESSION_ID_LEN+1) {
    return -1;
  }

  fd = open("/dev/urandom", O_RDONLY);
  if (fd  < 0) {
    return -1;
  }

  if (read(fd, b, 8) != 8) {
    return -1;
  }

  if (close(fd) < 0) {
    return -1;
  }

  for (i = 0; i < sizeof(b); ++i) {
    snprintf(buf + i * 2, len - i * 2, "%02X", (uint8_t)b[i]);
  }

  buf[GUAVA_SESSION_ID_LEN] = 0;
  return 0;
}
