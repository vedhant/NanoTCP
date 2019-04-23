#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "tcp.h"
#include "handle_user_calls.h"
#include "hash_map.h"

// FIXME: implement signals and error responses to user

// Random initial sequence number generator
uint32_t isn_generator()
{
  uint32_t isn = rand() & 0xff;
  isn |= (rand() & 0xff) << 8;
  isn |= (rand() & 0xff) << 16;
  isn |= (rand() & 0xff) << 24;
  return isn;
}

void handle_open(open_uc_info *open_info)
{
  // create connection id
  conn_id id;
  id.local.ip = local_ip;
  id.local.port = open_info->local_port;
  if (open_info->foreign_sock_specified)
  {
    id.remote = open_info->foreign_sock;
  }
  else
  {
    id.remote.ip = 0;
    id.remote.port = 0;
  }

  tcb connection;
  memset(&connection, 0, sizeof(tcb));
  int tcb_present_already = lookup_hashmap(id, &connection);
  printf("tcp looked up : %d\n", tcb_present_already);
  //CLOSED state
  if (tcb_present_already == -1)
  {
    // Connection requested is passive
    if (open_info->active == 0)
    {
      printf("passive\n");
      connection.id = id;
      if (open_info->foreign_sock_specified)
      {
        connection.foreign_sock_specified = 1;
      }
      connection.state = LISTEN;
      insert_hashmap(id, connection);
      return;
    }
    // Connection requested is active
    else
    {
      printf("active\n");
      if (open_info->foreign_sock_specified == 0)
      {
        printf("port %d didnt specify foreign sock for active opening", id.local.port);
        return;
      }
      //issue a SYN segment
      connection.id = id;
      connection.iss = isn_generator();

      printf("isn : %x\n", connection.iss);

      connection.snd_una = connection.iss;
      connection.snd_nxt = connection.iss + 1;
      connection.state = SYN_SENT;
      insert_hashmap(id, connection);

      tcp_hdr hdr;
      memset(&hdr, 0, sizeof(tcp_hdr));
      hdr.src_port = htons(id.local.port);
      hdr.dest_port = htons(id.remote.port);
      hdr.syn = 1;
      hdr.seq_no = htonl(connection.iss);
      hdr.data_offset = sizeof(hdr) / 4;
      hdr.checksum = htons(calc_checksum(hdr, id.local.ip, id.remote.ip));
      send_packets_to_ip(id.remote.ip, (char *)&hdr, sizeof(hdr));
    }
  }
}