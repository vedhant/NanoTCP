#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include "../socket/socket_tcp.h"

// maximum sequence number is 2**32 -1
#define MAX_SEQ_NO 4294967295

uint32_t local_ip;

// TCP Header Format

// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |          Source Port          |       Destination Port        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                        Sequence Number                        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                    Acknowledgment Number                      |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |  Data |           |U|A|P|R|S|F|                               |
// | Offset| Reserved  |R|C|S|S|Y|I|            Window             |
// |       |           |G|K|H|T|N|N|                               |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Checksum            |         Urgent Pointer        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                    Options                    |    Padding    |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                             data                              |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// Note : 6 bit reserved field is converted to 4 bit reserved field along with ece and cwr flags

// TODO: implement Options and Padding
typedef struct tcp_hdr
{
  uint16_t src_port;
  uint16_t dest_port;
  uint32_t seq_no;
  uint32_t ack_no;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  uint16_t reserved : 4,
           data_offset : 4,
           fin : 1,
           syn : 1,
           rst : 1,
           psh : 1,
           ack : 1,
           urg : 1,
           ece : 1,
           cwr : 1;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  uint16_t data_offset : 4,
           reserved : 4,
           cwr : 1,
           ece : 1,
           urg : 1,
           ack : 1,
           psh : 1,
           rst : 1,
           syn : 1,
           fin : 1;
#endif
  uint16_t window;
  uint16_t checksum;
  uint16_t urg_p;
  // FIXME: geting note in calc_checksum - the ABI of passing struct with a flexible array member has changed in GCC 4.4
  uint8_t data[];
} __attribute__((packed, aligned(1))) tcp_hdr;

// TCP pseudo header

//  +--------+--------+--------+--------+
//  |            Source Address         |                  
//  +--------+--------+--------+--------+
//  |          Destination Address      |
//  +--------+--------+--------+--------+
//  |  zero  |  PTCL  |    TCP Length   |
//  +--------+--------+--------+--------+

typedef struct tcp_pseudo_hdr
{
  uint32_t src_addr;
  uint32_t dest_addr;
  uint8_t zeros;
  uint8_t protocol;
  uint16_t tcp_len;
  tcp_hdr hdr;
} __attribute__((packed, aligned(1))) tcp_pseudo_hdr;

// Connection id : local and remote socket address pair uniquely identifies a connection

typedef struct conn_id
{
  sock_addr local;
  sock_addr remote;
} conn_id;

// TCP States

enum tcp_states
{
  LISTEN,
  SYN_SENT,
  SYN_RECEIVED,
  ESTABLISHED,
  FIN_WAIT_1,
  FIN_WAIT_2,
  CLOSE_WAIT,
  CLOSING,
  LAST_ACK,
  TIME_WAIT,
  CLOSED
};

// Transmission Control Block

typedef struct tcb
{
  conn_id id;

  int foreign_sock_specified;

  int state;

  char *usser_send_buff;
  char *usr_rcv_buff;

  // TODO: more fields to add, check rfc

  uint32_t snd_una;
  uint32_t snd_nxt;
  uint32_t snd_wnd;
  uint32_t snd_up;
  uint32_t snd_wl1;
  uint32_t snd_wl2;
  uint32_t iss;

  uint32_t rcv_nxt;
  uint32_t rcv_wnd;
  uint32_t rcv_up;
  uint32_t irs;
} tcb;

int send_packets_to_ip(uint32_t foreign_ip, char send_buffer[], int send_buff_size);
uint16_t calc_checksum(tcp_hdr hdr, uint32_t src_addr, uint32_t dest_addr);

#endif