#ifndef __SERVER_H__
#define __SERVER_H__

#include "common.h"

/**
 * @brief _client structure to make client list, USE ONLY for server-side.
 * @param fd file description
 */
typedef struct _client{
	int fd;
	struct _client *next;
} *p_client;

p_client Server_createClient(int _fd);
p_client Server_createList(void);
int Server_addList(p_client _client_list, int _fd);
int Server_removeList(p_client _client_list, int _fd);
int Server_showList(p_client _client_list);

int Server_init(void);
int Server_acceptConnect(int  listenFd, p_client _client_list);
int Server_sendAll(p_client _client_list, char *_message);

#endif
