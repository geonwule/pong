#include <iostream>
#include "Server.hpp"
#include "Util.hpp"

int main(int ac, char **av)
{
    std::signal(SIGINT, signalHandler); // Ctrl + C
    std::signal(SIGTERM, signalHandler); // kill
    std::signal(SIGPIPE, SIG_IGN);
    // std::signal(SIGPIPE, signalHandler); //SIGPIPE

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
