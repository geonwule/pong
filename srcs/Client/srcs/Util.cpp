#include "Util.hpp"
#include "Client.hpp"
#include "Thread.hpp"
#include "Cache.hpp"

void error_msg(enum e_error flag)
{
    if (errno)
    {
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
        exit(errno);
    }
    else if (flag == WRONG_ARG)
        std::cerr << "Error: " << "Wrong number of arguments" << std::endl;
    else if (flag == FATAL_ERR)
        std::cerr << "Error: " << "Fatal error" << std::endl;
    exit(EXIT_FAILURE);
}

void cleanMemory()
{
    Thread::cleanThread();

    Client *client = Client::getInstance();
    if (client)
        delete client;
}

void signalHandler(int signum) {
    if (signum == SIGINT)
        std::cout << "SIGINT: Interrupt signal received" << std::endl;
    else if (signum == SIGTERM)
        std::cout << "SIGTERM: Termination signal received" << std::endl;
    else
        std::cout << "Others: Signal " << signum << " received" << std::endl;
    Cache::atom_stop = true;
    cleanMemory();
    exit(signum);
}