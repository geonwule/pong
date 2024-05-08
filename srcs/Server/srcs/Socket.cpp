#include "Socket.hpp"
#include "Server.hpp"
#include "GameFrame.hpp"

void error_msg(enum e_error flag)
{
    if (flag == WRONG_ARG)
        write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
    else if (flag == FATAL_ERR)
        write(2, "Fatal error\n", strlen("Fatal error\n"));
    exit(1);
}

int extract_message(char **buf, char **msg)
{
    char *newbuf;
    int i;

    *msg = 0;
    if (*buf == 0)
        return (0);
    i = 0;
    while ((*buf)[i])
    {
        if ((*buf)[i] == '\n')
        {
            newbuf = (char *)calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
            if (newbuf == 0)
                error_msg(FATAL_ERR);
            strcpy(newbuf, *buf + i + 1);
            *msg = *buf;
            (*msg)[i + 1] = 0;
            *buf = newbuf;
            return (1);
        }
        i++;
    }
    return (0);
}

char *str_join(char *buf, char *add)
{
    char *newbuf;
    int len;

    if (buf == 0)
        len = 0;
    else
        len = strlen(buf);
    newbuf = (char *)malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
    if (newbuf == 0)
        error_msg(FATAL_ERR);
    newbuf[0] = 0;
    if (buf != 0)
    {
        strcat(newbuf, buf);
        free(buf);
    }
    strcat(newbuf, add);
    return (newbuf);
}