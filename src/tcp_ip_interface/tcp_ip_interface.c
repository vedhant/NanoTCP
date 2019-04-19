#include "tcp_ip_interface.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


// TODO: set appropriate MAX_RCV_BUFF_SIZE
#define MAX_RCV_BUFF_SIZE 1024
#define SA struct sockaddr

int raw_socket;

void init_tcp_ip_interface() {
  if((raw_socket = socket(AF_INET, SOCK_RAW, PROTOCOL_NO)) < 0) {
    perror("Error in initializing raw socket");
    exit(1);
  }

}

//TODO: look at the return type, check whether tcp.c knows the length of read buffer
char * receive_packets_from_ip(int * tcp_packet_size) {
  char * tcp_packet;
  tcp_packet = (char *) malloc(MAX_RCV_BUFF_SIZE * sizeof(char));
  memset(tcp_packet, 0, MAX_RCV_BUFF_SIZE);

  // receive the packet
  int size_read;
  size_read = read(raw_socket, tcp_packet, MAX_RCV_BUFF_SIZE);
  if(size_read < 0) {
    perror("error in receiving from raw socket");
    return NULL;
  }

  //get internet header
  in_hdr *rcv_in_hdr = (in_hdr *) tcp_packet;

  //test print
  printf("packet received : \n");
  for(int i=0; i<MAX_RCV_BUFF_SIZE; ++i) {
    printf("%x ", *(tcp_packet+i));
  }
  printf("\nsrc ip : %x, dest ip : %x\n", ntohl(rcv_in_hdr->src_addr), ntohl(rcv_in_hdr->dest_addr));
  printf("\ntotal packet length : %d\n", ntohs(rcv_in_hdr->total_len));
  printf("ip header length : %d\n", rcv_in_hdr->ihl * 4);

  //scrap of internet header
  tcp_packet += (rcv_in_hdr -> ihl) * 4;

  //set size of tcp packet
  * tcp_packet_size = ntohs(rcv_in_hdr -> total_len) - (rcv_in_hdr -> ihl) * 4;
  
  //return pointer to tcp packet buffer
  return tcp_packet;
}

int send_packets_to_ip(char foreign_ip[], char *send_buffer, int send_buff_size) {
  // TODO: cache the servaddr
  // setup target host address
  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  inet_pton(AF_INET, foreign_ip, &servaddr.sin_addr);
  if(connect(raw_socket, (SA *) &servaddr, sizeof(servaddr)) < 0) {
    perror("connection failed to foreign ip");
    return -1;
  }

  // send the packet
  int size_wrote;
  size_wrote = write(raw_socket, send_buffer, send_buff_size);
  if(size_wrote < 0) {
    perror("error in sending");
    return -2;
  }

  return size_wrote;
}