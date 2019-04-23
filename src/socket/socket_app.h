#ifndef SOCKET_APP_H
#define SOCKET_APP_H

#include "_socket_public.h"

uint32_t ip_to_int(const char * ip);
void open(uint16_t local_port, sock_addr *foreign_sock, uint8_t active);

#endif