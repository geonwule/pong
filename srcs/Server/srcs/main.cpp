#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "Socket.hpp"

s_Client clients[MAX_CLIENTS];
int next_id = 0;
int client_num = 0;

#include <thread>

#define MAX_THREAD 2
std::thread *thread_arr[MAX_THREAD];

void playGame();

#include <csignal>
#include <atomic>

int serv_fd;

void cleanMemory()
{
    std::cout << "cleanMemory()... start" << std::endl;
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (thread_arr[i] == nullptr)
            continue;
        std::cout << "thread_arr[" << i << "]->join()..." << std::endl;
        thread_arr[i]->join();
        delete thread_arr[i];
    }
    std::cout << "thread_arr[]->join()... done" << std::endl;

    close(serv_fd);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].fd > 0)
            close(clients[i].fd);
    }
}

std::atomic<bool> atom_stop(false);

void signalHandler(int signum) {
    if (signum == SIGINT)
        std::cout << "SIGINT: Interrupt signal received" << std::endl;
    else if (signum == SIGTERM)
        std::cout << "SIGTERM: Termination signal received" << std::endl;
    else
        std::cout << "Others: Signal " << signum << " received" << std::endl;
    atom_stop = true;
    std::cout << "signalHandler atom_stop = true" << std::endl;
    cleanMemory();
}

int main(int ac, char **av)
{
    std::signal(SIGINT, signalHandler); // Ctrl + C
    std::signal(SIGTERM, signalHandler); // kill

    for (int i = 0; i < MAX_THREAD; i++)
        thread_arr[i] = nullptr;

    if (ac != 2)
    {
        error_msg(WRONG_ARG);
    }

    int port_num = atoi(av[1]);
    int new_fd, len;
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
                    send_all(client.id, ARRIVE, NULL, clients);
                    client_num++;
                    if (client_num == 2)
                        thread_arr[0] = new std::thread(playGame);
                    // if (t1 != nullptr)
                    // {
                    //     t1->join();
                    //     delete t1;
                    //     t1 = nullptr;
                    // }
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
                    send_all(client.id, LEFT, NULL, clients);
                    client_num--;
                }
                else
                {
                    buff[read_bytes] = 0;
                    std::cout << "test: " << client.id << " :" << buff; // test
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
                        send_all(client.id, MSG, msg, clients);
                        free(msg);
                    }
                }
            }
        }
    }
}
