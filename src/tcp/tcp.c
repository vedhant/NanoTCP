#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
#include "../tcp_ip_interface/tcp_ip_interface.h"
#include "tcp.h"
#include "hash_map.h"
#include "handle_user_calls.h"

void init_tcp()
{
  local_ip = init_tcp_ip_interface();
  init_user_calls_msq();
  init_signals_msq();
  init_hash_table(1024);
  srand(time(0));
}

uint16_t calc_checksum(tcp_hdr hdr, uint32_t src_addr, uint32_t dest_addr)
{
  tcp_pseudo_hdr pseudo_hdr;
  pseudo_hdr.src_addr = src_addr;
  pseudo_hdr.dest_addr = ntohl(dest_addr);
  pseudo_hdr.zeros = 0;
  pseudo_hdr.protocol = PROTOCOL_NO;
  pseudo_hdr.tcp_len = sizeof(hdr);
  hdr.src_port = ntohs(hdr.src_port);
  hdr.dest_port = ntohs(hdr.dest_port);
  hdr.seq_no = ntohl(hdr.seq_no);
  pseudo_hdr.hdr = hdr;

  // Converting reserved, dataoffset and flags to big endian if system is little endian
  if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  {
    uint16_t *temp = (uint16_t *)&pseudo_hdr.hdr;
    temp += 6;
    *temp = ntohs(*temp);
  }

  // Converting zero and ptcl of pseudo header to big endian if system is little endian
  if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  {
    uint16_t *temp = (uint16_t *)&pseudo_hdr;
    temp += 4;
    *temp = ntohs(*temp);
  }

  size_t buff_size = sizeof(pseudo_hdr);
  uint16_t *buff = (uint16_t *)(&pseudo_hdr);

  unsigned long cksum = 0;
  while (buff_size > 1)
  {
    cksum += *buff;
    ++buff;
    buff_size -= sizeof(uint16_t);
  }
  if (buff_size)
  {
    cksum += *(uint8_t *)buff;
  }
  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);

  return (uint16_t)(~cksum);
}

// void test_send(char ip[]) {
//   tcp_hdr sample_hdr;
//   memset(&sample_hdr, 0, sizeof(sample_hdr));
//   sample_hdr.src_port = htons(8000);
//   sample_hdr.dest_port = htons(9877);
//   sample_hdr.seq_no = htons(5);
//   sample_hdr.ack_no = 6;
//   sample_hdr.data_offset = 5;
//   sample_hdr.syn = 1;
//   sample_hdr.window = 11;
//   sample_hdr.checksum = 0;

//   send_packets_to_ip(ip, (char *) &sample_hdr, sizeof(sample_hdr));
// }

// void test_receive() {
//   char * sample_hdr;
//   int packet_size;
//   sample_hdr = receive_packets_from_ip(&packet_size);

//   printf("\ntcp packet : \n");
//   for(int i=0; i<packet_size; ++i) {
//     printf("%x ", *(sample_hdr + i));
//   }
//   printf("\n");
// }

void *user_calls_thread()
{
  open_uc_info *open_uc;
  while (1)
  {
    open_uc = get_open_uc();
    if (open_uc != NULL)
    {
      handle_open(open_uc);
      free(open_uc);
      continue;
    }
  }
}

void *arriving_segments_thread()
{
}

void send_segment_thread()
{
}

int main(int argc, char **argv)
{
  init_tcp();

  pthread_t user_calls_threadid, arriving_seg_threadid;

  pthread_create(&user_calls_threadid, NULL, user_calls_thread, NULL);
  pthread_create(&arriving_seg_threadid, NULL, arriving_segments_thread, NULL);

  // test_receive();

  pthread_join(user_calls_threadid, NULL);
  pthread_join(arriving_seg_threadid, NULL);

  // test_send(argv[1]);
}