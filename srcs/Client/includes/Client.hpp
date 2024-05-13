#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <arpa/inet.h> //in_addr_t
#include <cstdlib>
#include <unistd.h> //close
#include "Paddle.hpp"
#define IP_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024


// Singleton
class Client
{

private:
    std::string _ip_address;
    int _port_num;
    int _fd;

    static Client *_instance;

    Client();
    Client(const Client &other);
    Client &operator=(const Client &other);
    Client(const char *port_num);
    Client(const char *ip, const char *port_num);

    /* Socket.cpp */
    // void bindAndListen();
    // void handleClientConnections();

public:
    ~Client();

    /* Client.cpp */
    static int setInstance(const char *port_num);
    static int setInstance(const char *ip, const char *port_num);
    static Client *getInstance();
    void connectServer();
    void handleServerCommunication();
    void sendPaddleDirection(e_paddle direction);

    /* Socket.cpp */
    // void runClient();
    // void sendClientMessage(int my_id, enum e_msg flag, char *msg);
    // void sendGameData(e_game flag, GameData *data = nullptr);
    // void receiveGameData(s_Client &player);
};

#endif // Client_HPP