#include "server.h"

/**
 * @brief Create a Client with next = NULL
 *
 * @param _fd file description of new client
 * @return <p_client>
 */
p_client Server_createClient(int _fd)
{
	p_client head = malloc(sizeof(p_client));
	head->fd = _fd;
	head->next = NULL;
	return head;
}

/**
 * @brief Create a new client list start from head
 *
 * @return <p_client> List_head
 */
p_client Server_createList(void)
{
	return Server_createClient(0);
}

/**
 * @brief add new client to list. 
 * create a new list if there's no list
 * @param _client_list active client list
 * @param _fd file description of client
 * @return <int> success or not
 */
int Server_addList(p_client _client_list, int _fd)
{
	if (_client_list == NULL)
	{
		_client_list = Server_createList();
		Server_addList(_client_list, _fd);
		return 1;
	}
	p_client _browse = _client_list;
	while (_browse->next != NULL)
	{
		_browse = _browse->next;
	}
	p_client _new = Server_createClient(_fd);
	_browse->next = _new;

	return 1;
}

/**
 * @brief remove a client from client list
 *
 * @param _client_list active client list
 * @param _fd file description of client need to be removed
 * @return <int> success or not
 */
int Server_removeList(p_client _client_list, int _fd)
{
	if (_client_list == NULL)
		return 0;
	p_client _browse1 = _client_list;
	p_client _browse2 = _browse1->next;
	while (_browse2 != NULL)
	{
		if (_browse2->fd == _fd)
		{
			_browse1->next = _browse2->next;
			free(_browse2);
			return 1;
		}
		_browse1 = _browse2;
		_browse2 = _browse2->next;
	}
	return 0;
}

/**
 * @brief Show the current client list
 *
 * @param _client_list active client list
 * @return <int> empty or not
 */
int Server_showList(p_client _client_list)
{
	if (_client_list == NULL || _client_list->next == NULL)
	{
		printf("Client list is empty");
		return 0;
	}

	p_client _browse = _client_list->next;
	while (_browse != NULL)
	{
		printf("%d -> ", _browse->fd);
		_browse = _browse->next;
	}
	printf("NULL\n");
	return 1;
}

/**
 * @brief Init the server
 * 
 * @return <int> _listen_fd, as a param of Server_acceptConnect() 
 */
int Server_init(void)
{
	int _listen_fd = 0;
	struct sockaddr_in _serv_addr;

	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_serv_addr, '0', sizeof(_serv_addr));

	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_serv_addr.sin_port = htons(SOCKET_PORT);

	/* Ignore pipe signals */
	signal(SIGPIPE, SIG_IGN);

	int option = 1;
	if(setsockopt(_listen_fd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0){
		perror("ERROR: setsockopt failed");
    	return EXIT_FAILURE;
	}

	bind(_listen_fd, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr));

	listen(_listen_fd, 10);

	return _listen_fd;
}

/**
 * @brief accept new connection and add new client to list
 * 
 * @param _listen_fd: file discriptor of server socket
 * @param _client_list: linked list of clients 
 * @return int: return file discriptor of neww socket that connect to client
 */
int Server_acceptConnect(int  _listen_fd, p_client _client_list)
{
	int _new_fd;
	struct sockaddr_in _new_addr;
	socklen_t _cli_len = sizeof(_new_addr);
	_new_fd = accept(_listen_fd, (struct sockaddr*)&_new_addr, &_cli_len);
	if(_new_fd >= 0)
	{
		Server_addList(_client_list, _new_fd);
	}
	return _new_fd;
}

/**
 * @brief send message to all client
 * 
 * @param _client_list current client list
 * @param _message message to send
 * @return <int> list is empty or not
 */
int Server_sendAll(p_client _client_list, char *_message)
{
	if (_client_list == NULL || _client_list->next == NULL)	return 0;

	p_client _browse = _client_list->next;
	while (_browse != NULL)
	{
		sendMess(_browse->fd, _message);
		_browse = _browse->next;
	}
	
	return 1;
}