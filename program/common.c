#include "common.h"

/**
 * @brief send mess (from buffer)
 * 
 * @param fd
 * @param buff 
 * @return void
 */
int sendMess(int fd, char *buff)
{
    return write(fd, buff, BUFFER_SIZE);
}

/**
 * @brief receive message
 * 
 * @param fd
 * @param buff 
 * @return void
 */
int recvMess(int fd, char *buff)
{
    return read(fd, buff, BUFFER_SIZE);
}