#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "common.h"

#define ROLE_LISTENER   1
#define ROLE_TALKER     2
#define ROLE_BOTH       3

struct client 
{
    int fd;
    int role;
    char name[NAME_SIZE];
    int connected;
    char send_buff[BUFFER_SIZE];
    char recv_buff[BUFFER_SIZE];
};

extern char input_buf[BUFFER_SIZE];

char getch();
void Client_showBuffer();                                     
void Client_printMessage(char *);               
void Client_processInput(char ch, void (*pfunc_process)(), void (*pfunc_exit)()); 

int Client_connect(void);
int Client_chooseRole(void);
void Client_getUserName(char *_pname);


#endif
