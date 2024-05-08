#ifndef SOCKET_HPP
#define SOCKET_HPP

#define MAX_CLIENTS 1024
#define BUFFER_SIZE 1024

#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/socket.h>

enum e_error
{
    WRONG_ARG,
    FATAL_ERR,
};

enum e_msg
{
    ARRIVE,
    LEFT,
    MSG,
};

struct s_Client
{
    int id, fd;
    char *buff;
};

void error_msg(enum e_error flag);
int extract_message(char **buf, char **msg);
char *str_join(char *buf, char *add);

#endif // SOCKET_HPP