#include "Client.hpp"
#include "Util.hpp"
#include "Thread.hpp"
#include <atomic>
#include "GameFrame.hpp"

extern std::atomic<bool> atom_stop;
extern GameData data;

Client *Client::_instance = nullptr;

Client::Client(const char *port_num) : _ip_address(IP_ADDRESS), _port_num(atoi(port_num))
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Client::Client(const char *ip_address, const char *port_num) : _ip_address(ip_address), _port_num(atoi(port_num))
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Client::~Client()
{
    close(_fd);
    // for (int i = 0; i < MAX_CLIENTS; i++)
    // {
    //     if (clients[i].fd > 0)
    //         close(clients[i].fd);
    // }
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

void Client::connectServer()
{
    // Create a socket
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1)
        error_msg(FATAL_ERR);

    // Set up the server address
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port_num);
    if (inet_pton(AF_INET, _ip_address.c_str(), &(serverAddr.sin_addr)) <= 0)
        error_msg(FATAL_ERR);

    // Connect to the server
    if (connect(_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        error_msg(FATAL_ERR);
}

static void threadReceiveMessage(int sockfd)
{
    char buffer[1024] = {0};
    bool isGameStart = false;
    while (!atom_stop)
    {
        if (!isGameStart)
        {
            int valread = recv(sockfd, buffer, 1024, 0);
            if (valread <= 0)
            {
                std::cerr << "Failed to receive message" << std::endl;
                // cleanMemory();
                // exit(1);
                atom_stop = true;
                return;
            }
            buffer[valread] = 0;
            std::cout << buffer << std::endl;
            if (std::string(buffer) == "Game Start")
            {
                std::cout << "Game Start" << std::endl;
                isGameStart = true;
            }
        }
        else
        {
            int valread = recv(sockfd, &data, sizeof(GameData), 0);
            // std::cout << "valread = " << valread << std::endl;
            // std::cout << "sizeof(GameData) = " << sizeof(GameData) << std::endl;
            // if (valread != sizeof(GameData)) // 네트워크 전송에 문제가 있을떄 다르게 받아옴
            if (valread <= 0)
            {
                std::cerr << "Failed to receive GameData" << std::endl;
                // cleanMemory();
                // exit(1);
                atom_stop = true;
                return;
            }
            // std::cout << "Received GameData" << std::endl;
        }
    }
}

static void threadSendMessage(int sockfd)
{
    while (!atom_stop)
    {
        fd_set set;
        struct timeval timeout;
        int rv;
        char buff[100];
        FD_ZERO(&set);   /* clear the set */
        FD_SET(0, &set); /* add our file descriptor to the set */

        timeout.tv_sec = 0;
        timeout.tv_usec = 500000; // 0.5 seconds

        rv = select(1, &set, NULL, NULL, &timeout);

        if (rv == -1)
            perror("select"); /* an error accured */
        else if (rv == 0)
            continue; /* a timeout occured */
        else
            read(0, buff, sizeof buff); /* there was data to read */

        std::string message(buff);
        if (message == "exit")
        {
            atom_stop = true;
            break;
        }

        message += '\n';

        if (send(sockfd, message.c_str(), message.size(), 0) == -1)
        {
            std::cerr << "Failed to send message" << std::endl;
            atom_stop = true;
            return;
        }
        else
        {
            std::cout << "Message sent: " << message << std::endl;
        }
    }
}

void Client::handleServerCommunication()
{
    Thread::createThread(threadReceiveMessage, _fd);
    Thread::createThread(threadSendMessage, _fd);
}

void Client::sendPaddleDirection(e_paddle direction)
{
    std::string message;
    if (direction == UP)
        message = "UP";
    else if (direction == DOWN)
        message = "DOWN";
    else
        return;
    if (send(_fd, message.c_str(), message.size(), 0) == -1)
        error_msg(FATAL_ERR);
    std::cout << "Message sent: " << message << std::endl;
}
