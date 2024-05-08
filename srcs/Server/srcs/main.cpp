#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "Socket.hpp"
#include "Server.hpp"

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

void cleanThread()
{
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (thread_arr[i] == nullptr)
            continue;
        std::cout << "thread_arr[" << i << "]->join()..." << std::endl;
        thread_arr[i]->join();
        delete thread_arr[i];
        thread_arr[i] = nullptr;
        std::cout << "thread_arr[" << i << "]->join()... done" << std::endl;
    }
}

void cleanMemory()
{
    std::cout << "cleanMemory()... start" << std::endl;
    cleanThread();
    // for (int i = 0; i < MAX_THREAD; i++)
    // {
    //     if (thread_arr[i] == nullptr)
    //         continue;
    //     std::cout << "thread_arr[" << i << "]->join()..." << std::endl;
    //     thread_arr[i]->join();
    //     delete thread_arr[i];
    // }
    std::cout << "thread_arr[]->join()... done" << std::endl;

    Server *server = Server::getInstance();
    if (server)
        delete server;
}

std::atomic<bool> atom_stop(false);

void signalHandler(int signum) {
    if (signum == SIGINT)
        std::cout << "SIGINT: Interrupt signal received" << std::endl;
    else if (signum == SIGTERM)
        std::cout << "SIGTERM: Termination signal received" << std::endl;
    else if (signum == SIGPIPE)
    {
        std::cout << "SIGPIPE: Broken pipe signal received" << std::endl;
        atom_stop = true;
        cleanThread();
        return ;
    }
    else
        std::cout << "Others: Signal " << signum << " received" << std::endl;
    atom_stop = true;
    std::cout << "signalHandler atom_stop = true" << std::endl;
    cleanMemory();
    exit(signum);
}

int main(int ac, char **av)
{
    std::signal(SIGINT, signalHandler); // Ctrl + C
    std::signal(SIGTERM, signalHandler); // kill
    std::signal(SIGPIPE, signalHandler); //SIGPIPE

    for (int i = 0; i < MAX_THREAD; i++)
        thread_arr[i] = nullptr;

    if (!(ac == 2 || ac == 3))
    {
        std::cerr << "input:[ip] <port>" << std::endl;
        error_msg(WRONG_ARG);
    }

    if (ac == 2)
        Server::setInstance(av[1]);
    else // ac == 3
        Server::setInstance(av[1], av[2]);
    
    Server *server = Server::getInstance();
    server->runServer();
}
