#include "Util.hpp"
#include "Server.hpp"
#include "Thread.hpp"
#include <atomic>

extern std::atomic<bool> atom_stop;

void cleanMemory()
{
    Thread::cleanThread();

    Server *server = Server::getInstance();
    if (server)
        delete server;
}

void signalHandler(int signum) {
    if (signum == SIGINT)
        std::cout << "SIGINT: Interrupt signal received" << std::endl;
    else if (signum == SIGTERM)
        std::cout << "SIGTERM: Termination signal received" << std::endl;
    else if (signum == SIGPIPE)
    {
        std::cout << "SIGPIPE: Broken pipe signal received" << std::endl;
        atom_stop = true;
        Thread::cleanThread();
        return ;
    }
    else
        std::cout << "Others: Signal " << signum << " received" << std::endl;
    atom_stop = true;
    std::cout << "signalHandler atom_stop = true" << std::endl;
    cleanMemory();
    exit(signum);
}