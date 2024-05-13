#include <csignal> //std::signal

#include "Util.hpp"
#include "Client.hpp"
#include "Cache.hpp"

void rendering();

int main(int ac, char **av)
{
    std::signal(SIGINT, signalHandler);  // Ctrl + C
    std::signal(SIGTERM, signalHandler); // kill

    if (!(ac == 2 || ac == 3))
        error_msg(WRONG_ARG);

    if (ac == 2)
        Client::setInstance(av[1]);
    else // ac == 3
        Client::setInstance(av[1], av[2]);

    Client *client = Client::getInstance();
    if (client == nullptr)
        error_msg(FATAL_ERR);

    client->runClient();

    while (!Cache::atom_stop)
    {
        if (Cache::isGameStart)
            rendering();
        // std::cout << "Waiting for the game to start..." << std::endl;
    }
    cleanMemory();
    return 0;
}