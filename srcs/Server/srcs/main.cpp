#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#define MAX_CLIENTS 1024
#define BUFFER_SIZE 1024

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

s_Client clients[MAX_CLIENTS];
int next_id = 0;

void error_msg(enum e_error flag)
{
    if (flag == WRONG_ARG)
        write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
    else if (flag == FATAL_ERR)
        write(2, "Fatal error\n", strlen("Fatal error\n"));
    exit(1);
}

void send_all(int my_id, enum e_msg flag, char *msg)
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

int main(int ac, char **av)
{
    if (ac != 2)
    {
        error_msg(WRONG_ARG);
    }

    int port_num = atoi(av[1]);
    int serv_fd, new_fd, len;
    struct sockaddr_in servaddr, cli;
    // socket create and verification

    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd == -1)
    {
        error_msg(FATAL_ERR);
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
    servaddr.sin_port = htons(port_num);

    // Binding newly created socket to given IP and verification
    if ((bind(serv_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    {
        error_msg(FATAL_ERR);
    }

    if (listen(serv_fd, 10) != 0)
    {
        error_msg(FATAL_ERR);
    }

    fd_set read_fds;
    int max_fd;
    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(serv_fd, &read_fds);
        max_fd = serv_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i].fd > 0)
            {
                FD_SET(clients[i].fd, &read_fds);
                if (clients[i].fd > max_fd)
                    max_fd = clients[i].fd;
            }
        }

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
            continue;

        if (FD_ISSET(serv_fd, &read_fds))
        {
            len = sizeof(cli);
            new_fd = accept(serv_fd, (struct sockaddr *)&cli, (socklen_t *)&len);
            if (new_fd < 0)
                error_msg(FATAL_ERR);
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                s_Client &client = clients[i];
                if (client.fd == 0)
                {
                    client.fd = new_fd;
                    client.id = next_id++;
                    client.buff = NULL;
                    send_all(client.id, ARRIVE, NULL);
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            s_Client &client = clients[i];
            if (FD_ISSET(client.fd, &read_fds))
            {
                char buff[BUFFER_SIZE];
                int read_bytes = recv(client.fd, buff, BUFFER_SIZE - 1, 0);
                if (read_bytes <= 0)
                {
                    close(client.fd);
                    clients[i].fd = 0;
                    send_all(client.id, LEFT, NULL);
                }
                else
                {
                    buff[read_bytes] = 0;
                    std::cout << "test: " << client.id << " :" <<buff; //test
                    client.buff = str_join(client.buff, buff);
                    while (read_bytes == BUFFER_SIZE - 1)
                    {
                        read_bytes = recv(client.fd, buff, BUFFER_SIZE - 1, 0);
                        if (read_bytes <= 0)
                            break;
                        buff[read_bytes] = 0;
                        client.buff = str_join(client.buff, buff);
                    }
                    char *msg;
                    while (extract_message(&clients[i].buff, &msg) > 0)
                    {
                        send_all(client.id, MSG, msg);
                        free(msg);
                    }
                }
            }
        }
    }
}
