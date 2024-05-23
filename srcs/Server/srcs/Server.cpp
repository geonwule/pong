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
    
    for (int i = 0; i < MAX_CLIENTS; i++)
        clients[i].fd = 0;
}

Server::Server(const char *ip_address, const char *port_num) : _ip_address(inet_addr(ip_address)), _port_num(atoi(port_num)), serv_fd(0)
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }

    for (int i = 0; i < MAX_CLIENTS; i++)
        clients[i].fd = 0;
}

Server::~Server()
{
    if (serv_fd)
        close(serv_fd);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].buff)
            delete clients[i].buff;
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

s_Client *Server::getPlayer()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].fd > 0 && clients[i].waiting_game)
        {
            clients[i].waiting_game = 0;
            clients[i].is_game_ing = true;
            return &clients[i];
        }
    }
    return nullptr;
}
