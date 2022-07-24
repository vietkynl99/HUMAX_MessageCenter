#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h> 
#include <signal.h>
#include <unistd.h>  
#include <termios.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <signal.h>


#define EXIT_MESSAGE            ":*@#!$^q"
#define BUFFER_SIZE             50
#define NAME_SIZE               50
#define SOCKET_PORT             5000

int sendMess(int fd, char *buff);
int recvMess(int fd, char *buff);


#endif
