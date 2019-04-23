#include <stdlib.h>
#include "tcp.h"
#include "hash_map.h"

typedef struct node {
  conn_id id;
  tcb connection;
  struct node * next;
} node;

typedef struct table {
  int size;
  node ** list;
} table;

table *t;

void init_hash_table(int size) {
  t = (table *) malloc(sizeof(table));
  t->size = size;
  t->list = (node **) malloc(sizeof(node *) * size);
  for(int i=0; i<size; ++i)
    t->list[i] = NULL;
}

int hashCode(conn_id id) {
  uint64_t key = 0;
  key = id.local.port;
  key = key << 48;
  key |= id.remote.ip;
  key = key << 16;
  key |= id.remote.port;

  // we will not use local ip as it will be common in all
  return key % t->size;
}

int cmp_conn_id(conn_id id1, conn_id id2) {
  return ((id1.local.ip == id2.local.ip) && (id1.local.port == id2.local.port) &&
          (id1.remote.ip == id2.remote.ip) && (id1.remote.port == id2.remote.port));
}

void insert_hashmap(conn_id id, tcb connection) {
  int pos = hashCode(id);
  node * list =  t->list[pos];
  node * newNode = (node *) malloc(sizeof(node));
  node * temp = list;
  while(temp) {
    if(cmp_conn_id(temp->id, id)) {
      free(newNode);
      temp->connection = connection;
      return;
    }
    temp = temp->next;
  }
  newNode->id = id;
  newNode->connection = connection;
  newNode->next = list;
  t->list[pos] = newNode;
  return;
}

int lookup_hashmap(conn_id id, tcb * connection) {
  int pos = hashCode(id);
  node * list =  t->list[pos];
  node * temp = list;
  while(temp) {
    if(cmp_conn_id(temp->id, id)) {
      *connection = temp->connection;
      return 0;
    }
    temp = temp->next;
  }
  return -1;
}