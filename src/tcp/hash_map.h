#ifndef HASH_MAP_H
#define HASH_MAP_H

void init_hash_table(int size);
void insert_hashmap(conn_id id, tcb connection);
int lookup_hashmap(conn_id id, tcb * connection);

#endif