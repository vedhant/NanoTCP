#ifndef _SOCKET_PRIVATE_H
#define _SOCKET_PRIVATE_H

#include <stdint.h>
#include "_socket_public.h"

typedef struct open_uc_info {
  uint16_t local_port;
  int foreign_sock_specified;
  sock_addr foreign_sock;
  uint8_t active;
} open_uc_info;

typedef struct open_uc_msgbuf {
  long mtype;
  open_uc_info mdata;
} open_uc_msgbuf;

#endif