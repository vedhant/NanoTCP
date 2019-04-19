#include "tcp.h"
#include "../tcp_ip_interface/tcp_ip_interface.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

void init_tcp() {
  init_tcp_ip_interface();
}

void test_send(char ip[]) {
  tcp_hdr sample_hdr;
  memset(&sample_hdr, 0, sizeof(sample_hdr));
  sample_hdr.src_port = htons(8000);
  sample_hdr.dest_port = htons(9877);
  sample_hdr.seq_no = htons(5);
  sample_hdr.ack_no = 6;
  sample_hdr.data_offset = 5;
  sample_hdr.syn = 1;
  sample_hdr.window = 11;
  sample_hdr.checksum = 0;

  send_packets_to_ip(ip, (char *) &sample_hdr, sizeof(sample_hdr));
}

void test_receive() {
  char * sample_hdr;
  int packet_size;
  sample_hdr = receive_packets_from_ip(&packet_size);

  printf("\ntcp packet : \n");
  for(int i=0; i<packet_size; ++i) {
    printf("%x ", *(sample_hdr + i));
  }
  printf("\n");
}

int main(int argc, char **argv) {
  init_tcp();

  // test_send(argv[1]);
  test_receive();
}