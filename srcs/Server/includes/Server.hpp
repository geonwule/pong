#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <cstdlib>
#include "Socket.hpp"

#define IP_ADDRESS "127.0.0.1"

// Singleton
class Server
{

private:
    in_addr_t _ip_address;
    int _port_num;
    int serv_fd;
    
    // struct timeval timeout;

    s_Client clients[MAX_CLIENTS];
    int next_id = 0;
    int client_num = 0;

    static Server *_instance;

    Server();
    Server(const Server &other);
    Server &operator=(const Server &other);
    Server(const char *port_num);
    Server(const char *ip, const char *port_num);

    void bindAndListen();
    void handleClientConnections();

public:
    ~Server();

    void runServer();
    static int setInstance(const char *port_num);
    static int setInstance(const char *ip, const char *port_num);
    static Server *getInstance();
};

#endif // SERVER_HPP