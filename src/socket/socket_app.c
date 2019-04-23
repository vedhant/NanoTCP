#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "_socket_private.h"
#include "socket_app.h"

#define USER_CALLS_KEY_FILE "src/socket/user_calls_key"
#define SIGNALS_KEY_FILE "src/socket/signals_key"

int user_calls_msqid;
int signals_mswid;
int msgq_connected = 0;

void connect_user_calls_msgq() {
  key_t key = ftok(USER_CALLS_KEY_FILE, 'T');
  if(key < 0) {
    perror("could not create key for user calls msgq");
    exit(1);
  }

  user_calls_msqid = msgget(key, 0);
  if(user_calls_msqid < 0) {
    perror("could not connect to msgq for user calls");
    exit(1);
  }
}

void connect_signals_msq() {
  key_t key = ftok(SIGNALS_KEY_FILE, 'C');
  if(key < 0) {
    perror("could not create key for signals msgq");
    exit(1);
  }

  signals_mswid = msgget(key, 0);
  if(signals_mswid < 0) {
    perror("could not connect to msgq for signals");
    exit(1);
  }
}

uint32_t ip_to_int(const char * ip) {
  uint32_t temp = 0;
  uint32_t * ip_in_int = &temp;
  // FIXME: check byte ordering
  inet_pton(AF_INET, ip, ip_in_int);
  return * ip_in_int;
}

void open(uint16_t local_port, sock_addr *foreign_sock, uint8_t active) {
  if(msgq_connected == 0) {
    connect_user_calls_msgq();
    connect_signals_msq();
  }
  
  open_uc_msgbuf msgbuf;
  msgbuf.mtype = 1;
  msgbuf.mdata.local_port = local_port;
  if(foreign_sock != NULL) {
    msgbuf.mdata.foreign_sock = *foreign_sock;
    msgbuf.mdata.foreign_sock_specified = 1;
  }
  else {
    msgbuf.mdata.foreign_sock_specified = 0;
  }
  msgbuf.mdata.active = active;

  msgsnd(user_calls_msqid, &msgbuf, sizeof(open_uc_info), 0);
}