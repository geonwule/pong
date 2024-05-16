#include "Client.hpp"
#include "Util.hpp"
#include "Thread.hpp"
#include "Cache.hpp"

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
    GameData &data = Cache::data;

    char buffer[BUFFER_SIZE] = {0};
    while (!Cache::atom_stop)
    {
        if (!Cache::isGameStart)
        {
            int valread = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
            if (valread <= 0)
            {
                std::cerr << "Failed to receive message" << std::endl;
                // cleanMemory();
                // exit(1);
                Cache::atom_stop = true;
                return;
            }
            buffer[valread] = 0;
            std::cout << buffer << std::endl;
            if (std::string(buffer) == "Game Start")
            {
                std::cout << "Game Start" << std::endl;
                Cache::isGameStart = true;
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
                Cache::atom_stop = true;
                return;
            }
            if (data.isGameStart == GAME_END)
            {
                Cache::isGameStart = false;
                continue;
            }
            // std::cout << "Received GameData" << std::endl;
        }
    }
}

static void threadSendMessage(int sockfd)
{
    while (!Cache::atom_stop)
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

        ssize_t read_bytes = 0;
        if (rv == -1)
            perror("select"); /* an error accured */
        else if (rv == 0)
            continue; /* a timeout occured */
        else
            read_bytes = read(0, buff, sizeof buff); /* there was data to read */
        buff[read_bytes] = 0;

        std::string message(buff);
        if (message.compare("exit\n") == 0)
        {
            std::cout << "Exit the game" << std::endl;
            Cache::atom_stop = true;
            break;
        }
        if (send(sockfd, message.c_str(), message.size(), 0) == -1)
        {
            std::cerr << "Failed to send message" << std::endl;
            Cache::atom_stop = true;
            return;
        }
    }
}

void Client::runClient()
{
    connectServer();
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
