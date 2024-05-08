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


#include <csignal>
#include <atomic>

std::atomic<bool> atom_stop(false);


#include "Util.hpp"


int main(int ac, char **av)
{
    std::signal(SIGINT, signalHandler); // Ctrl + C
    std::signal(SIGTERM, signalHandler); // kill
    std::signal(SIGPIPE, signalHandler); //SIGPIPE

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
