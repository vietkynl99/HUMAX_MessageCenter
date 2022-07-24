#include "server.h"
#include "common.h"

p_client gclient_list;
int glisten_fd;

void *ClientHandle(void *arg_client_fd)
{
    int n;
    int client_fd = *((int *)arg_client_fd);
    char _recv_buf[BUFFER_SIZE];
    while (1)
    {
		memset(_recv_buf, 0, BUFFER_SIZE);
        n = recvMess(client_fd, _recv_buf);
        if(n < 0)
		{
			//perror("Error while receiving data from client");
			//exit(1);
            break;
		}
		else if(n == 0 || !strcmp(_recv_buf, EXIT_MESSAGE))
		{
			Server_removeList(gclient_list, client_fd);
            printf("client fd %d has letf\n", client_fd);
            break;
		}
		else
		{
			printf("client fd %d: %s\n", client_fd, _recv_buf);
            Server_sendAll(gclient_list, _recv_buf);
		}
    }
    return arg_client_fd;   
}
void ctr_C_handler(sig_t s)
{
    Server_sendAll(gclient_list, "[Message center]: server has stopped working!");
	exit(0);
}

int main()
{
    pthread_t tid;
    int new_client_fd;
    
    //hanlde ctrl-C signal
	signal (SIGINT, ctr_C_handler);
    //create list
    printf("\n--- HUMAX CHAT APP SERVER ----\n");
    gclient_list = Server_createList();
    //init server
    glisten_fd = Server_init();

    while (1)
    {
        //accpet connection from client
        new_client_fd = Server_acceptConnect(glisten_fd, gclient_list);
        if(new_client_fd < 0)
        {
            printf("The number of guests has reached the limit. server can't work\n\n");
            return 1;
        }
        printf("Accecpt new client fd %d\n", new_client_fd);
        //create new thread for each client
        pthread_create(&tid, NULL, ClientHandle, (void *)&new_client_fd);
    }


    
    printf("Server is closed!\n");
    return 0;
}
