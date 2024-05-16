#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <arpa/inet.h> //in_addr_t
#include <cstdlib>
#include <unistd.h> //close

#define IP_ADDRESS "127.0.0.1"
#define MAX_CLIENTS 1024
#define BUFFER_SIZE 1024

enum e_msg
{
    ARRIVE,
    LEFT,
    MSG,
};

enum e_game
{
    GAME_START,
    GAME_LOADING,
    GAME_ING,
    GAME_END,
};

struct s_Client
{
    int id, fd;
    char *buff;
    int waiting_game;
    std::string msg;
};

struct GameData;
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

    /* Socket.cpp */
    void bindAndListen();
    void handleClientConnections();

    /* handleClientConnections() */
    void prepareReadFds(fd_set &read_fds, int &max_fd);
    void acceptNewClient(struct sockaddr_in &cli);
    void handleClientMessage(fd_set &read_fds);

public:
    ~Server();

    /* Server.cpp */
    static int setInstance(const char *port_num);
    static int setInstance(const char *ip, const char *port_num);
    static Server *getInstance();
    s_Client *getPlayer();

    /* Socket.cpp */
    void runServer();
    void sendClientMessage(int my_id, enum e_msg flag, char *msg);
    void sendGameData(e_game flag, int *players_id, GameData *data = nullptr);
    int receiveGameData(s_Client &player, int &isGameStart);
};

#endif // SERVER_HPP