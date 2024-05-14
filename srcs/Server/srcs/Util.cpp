#include "Util.hpp"
#include "Server.hpp"
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
        std::cout << "SIGPIPE: Broken pipe signal received\n";
        Cache::atom_stop = true;
        Thread::cleanThread();
        return ;
    }
    else
        std::cout << "Others: Signal " << signum << " received" << std::endl;
    cleanMemory();
    exit(signum);
}