#include "Server.hpp"
#include "Util.hpp"

Server *Server::_instance = nullptr;

Server::Server(const char *port_num) : _ip_address(inet_addr(IP_ADDRESS)), _port_num(atoi(port_num)), serv_fd(0)
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Server::Server(const char *ip_address, const char *port_num) : _ip_address(inet_addr(ip_address)), _port_num(atoi(port_num)), serv_fd(0)
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Server::~Server()
{
    if (serv_fd)
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

s_Client *Server::getClient()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].fd > 0 && !clients[i].game_ing)
        {
            clients[i].game_ing = 1;
            return &clients[i];
        }
    }
    return nullptr;
}
