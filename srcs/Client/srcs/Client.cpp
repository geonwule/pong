#include "Client.hpp"
#include "Util.hpp"

Client *Client::_instance = nullptr;

Client::Client(const char *port_num) : _ip_address(IP_ADDRESS), _port_num(atoi(port_num)), _fd(0)
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Client::Client(const char *ip_address, const char *port_num) : _ip_address(ip_address), _port_num(atoi(port_num)), _fd(0)
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Client::~Client()
{
    if (_fd)
        close(_fd);
}

int Client::setInstance(const char *port_num)
{
    if (_instance == nullptr)
    {
        _instance = new Client(port_num);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int Client::setInstance(const char *ip, const char *port_num)
{
    if (_instance == nullptr)
    {
        _instance = new Client(ip, port_num);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

Client *Client::getInstance()
{
    return _instance;
}
