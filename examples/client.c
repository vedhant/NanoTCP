#include <stdint.h>
#include <stdio.h>
#include "../src/socket/socket_app.h"

int main() {
  uint16_t local_port = 9877;
  sock_addr foreign_sock;
  foreign_sock.port = 4000;
  foreign_sock.ip = ip_to_int("10.42.0.204");

  // printf("\n%d\n", foreign_sock.ip);
  open(local_port, &foreign_sock, 1);

  foreign_sock.port = 8000;
  foreign_sock.ip = ip_to_int("10.42.0.159");
  open(local_port, &foreign_sock, 1);
}