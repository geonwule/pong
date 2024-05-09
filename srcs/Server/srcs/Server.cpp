#include "Server.hpp"
#include "Util.hpp"

Server *Server::_instance = nullptr;

Server::Server(const char *port_num) : _ip_address(inet_addr(IP_ADDRESS)), _port_num(atoi(port_num))
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Server::Server(const char *ip_address, const char *port_num) : _ip_address(inet_addr(ip_address)), _port_num(atoi(port_num))
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Server::~Server()
{
    close(serv_fd);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].fd > 0)
            close(clients[i].fd);
    }
}

int Server::setInstance(const char *port_num)
{
    if (_instance == nullptr)
    {
        _instance = new Server(port_num);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int Server::setInstance(const char *ip, const char *port_num)
{
    if (_instance == nullptr)
    {
        _instance = new Server(ip, port_num);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

Server *Server::getInstance()
{
    return _instance;
}
