#include "Socket.hpp"

void error_msg(enum e_error flag)
{
    if (flag == WRONG_ARG)
        write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
    else if (flag == FATAL_ERR)
        write(2, "Fatal error\n", strlen("Fatal error\n"));
    exit(1);
}

void send_all(int my_id, enum e_msg flag, char *msg, s_Client *clients)
{
    char buff[BUFFER_SIZE];
    int read_bytes;
    if (flag == ARRIVE)
    {
        read_bytes = sprintf(buff, "server: client %d just arrived\n", my_id);
    }
    else if (flag == LEFT)
    {
        read_bytes = sprintf(buff, "server: client %d just left\n", my_id);
    }
    else if (flag == MSG)
    {
        read_bytes = sprintf(buff, "client %d: ", my_id);
    }

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        s_Client &client = clients[i];
        if (client.fd > 0 && client.id != my_id)
        {
            int bytes_sent = send(client.fd, buff, read_bytes, 0);
            if (bytes_sent == -1)
            {
                std::cerr << "Failed to send message to client " << client.id << std::endl;
                continue;
            }
            if (flag == MSG)
            {
                bytes_sent = send(client.fd, msg, strlen(msg), 0);
                if (bytes_sent == -1)
                {
                    std::cerr << "Failed to send message to client " << client.id << std::endl;
                }
            }
        }
    }
    // test
    std::cout << buff;
    if (flag == MSG)
        std::cout << msg;
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