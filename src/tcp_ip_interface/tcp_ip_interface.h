#ifndef TCP_IP_INTERFACE_H
#define TCP_IP_INTERFACE_H

#include <stdint.h>

#define PROTOCOL_NO 6

extern int raw_socket;

void init_tcp_ip_interface();
char * receive_packets_from_ip(int * tcp_packet_size);
int send_packets_to_ip(char foreign_ip[], char send_buffer[], int send_buff_size);

// Internet Header Format

// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |Version|  IHL  |Type of Service|          Total Length         |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |         Identification        |Flags|      Fragment Offset    |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |  Time to Live |    Protocol   |         Header Checksum       |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                       Source Address                          |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                    Destination Address                        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                    Options                    |    Padding    |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

typedef struct in_hdr {
  # if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    uint8_t version: 4,
            ihl: 4;
  # elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t ihl: 4,
            version: 4;
  # endif
    uint8_t type_of_service;
    uint16_t total_len;
    uint16_t identification;
    uint16_t flags: 3,
            frag_offset: 13;
    uint8_t time_to_live;
    uint8_t protocol;
    uint16_t hdr_checksum;
    uint32_t src_addr;
    uint32_t dest_addr;
}__attribute__((packed, aligned(1))) in_hdr;

#endif