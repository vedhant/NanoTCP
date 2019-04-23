#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>
#include "socket_tcp.h"

#define USER_CALLS_KEY_FILE "src/socket/user_calls_key"
#define SIGNALS_KEY_FILE "src/socket/signals_key"

int user_calls_msqid;
int signals_mswid;

void init_user_calls_msq() {
  key_t key = ftok(USER_CALLS_KEY_FILE, 'T');
  if(key < 0) {
    perror("could not create key for user calls msgq");
    exit(1);
  }

  user_calls_msqid = msgget(key, IPC_CREAT | 0666);
  if(user_calls_msqid < 0) {
    perror("could not create msgq for user calls");
    exit(1);
  }
}

void init_signals_msq() {
  key_t key = ftok(SIGNALS_KEY_FILE, 'C');
  if(key < 0) {
    perror("could not create key for signals msgq");
    exit(1);
  }

  signals_mswid = msgget(key, IPC_CREAT | 0666);
  if(signals_mswid < 0) {
    perror("could not create msgq for signals");
    exit(1);
  }
}

open_uc_info * get_open_uc() {
  open_uc_msgbuf msgbuf;
  open_uc_info *msgdata = (open_uc_info *) malloc(sizeof(open_uc_info));
  if(msgrcv(user_calls_msqid, &msgbuf, sizeof(open_uc_info), 1, 0) < 0) {
    return NULL;
  }
  *msgdata = msgbuf.mdata; 
  return msgdata;
}