#ifndef _SOCKET_PUBLIC_H
#define _SOCKET_PUBLIC_H

#include <stdint.h>

// Socket address : uniquely identifies a process

typedef struct sock_addr {
  uint32_t ip;
  uint16_t port;
} sock_addr;

#endif