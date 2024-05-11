#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <future>
#include <thread>
// #include "../../../mac_opengl/include/GLFW/glfw3.h"

#define WIDTH 2000
#define HEIGHT 1000
enum e_game
{
    GAME_START,
    GAME_LOADING,
    GAME_ING,
    GAME_END,
};
using namespace std;
#include <sstream>
#include <cmath>
#include "GameFrame.hpp"

#include <csignal>
#include <cstring>
#include <atomic>

void rendering(GameData &data);

GameData data = {
    0.5f, 0.5f, 0.02f,
    0.02f, 0.2f, 0.9f, 0.5f, 0.1f, 0.5f,
    3, 3,
    0}; // temp

#define MAX_THREAD 2
std::thread *thread_arr[MAX_THREAD];
int sockfd;

enum e_paddle
{
    UP,
    DOWN,
    STOP,
    PLAYER1,
    PLAYER2
};
void sendMessage(e_paddle direction)
{
    std::string message;
    if (direction == UP)
        message = "UP";
    else if (direction == DOWN)
        message = "DOWN";
    else
        return;
    if (send(sockfd, message.c_str(), message.size(), 0) == -1)
    {
        cerr << "Failed to send message" << endl;
        exit(1);
    }
    std::cout << "Message sent: " << message << std::endl;
}

void cleanMemory()
{
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (thread_arr[i] == nullptr)
            continue;
        thread_arr[i]->join();
        delete thread_arr[i];
    }
    close(sockfd);
    std::cout << "Memory cleaned" << std::endl;
}

std::atomic<bool> atom_stop(false);

void signalHandler(int signum)
{
    if (signum == SIGINT)
        std::cout << "SIGINT: Interrupt signal received" << std::endl;
    else if (signum == SIGTERM)
        std::cout << "SIGTERM: Termination signal received" << std::endl;
    else
        std::cout << "Others: Signal " << signum << " received" << std::endl;
    atom_stop = true;
    cleanMemory();
    exit(signum);
}

void threadReceiveMessage(int sockfd)
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
                cerr << "Failed to receive message" << endl;
                // cleanMemory();
                // exit(1);
                atom_stop = true;
                return;
            }
            buffer[valread] = 0;
            cout << buffer << endl;
            if (string(buffer) == "Game Start")
                isGameStart = true;
        }
        else
        {
            int valread = recv(sockfd, &data, sizeof(GameData), 0);
            // std::cout << "valread = " << valread << std::endl;
            // std::cout << "sizeof(data) = " << sizeof(GameData) << std::endl;
            if (valread != sizeof(GameData))
            // if (valread <= 0)
            {
                cerr << "Failed to receive GameData" << endl;
                // cleanMemory();
                // exit(1);
                atom_stop = true;
                return;
            }
            // std::cout << "Received GameData" << std::endl;
        }
    }
}

void threadSocketNetwork(int sockfd)
{
    while (!atom_stop)
    {
        // string message;
        // cout << "Enter message: ";
        // getline(cin, message);

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
            cerr << "Failed to send message" << endl;
            atom_stop = true;
            return;
        }
        else
        {
            cout << "Message sent: " << message << endl;
        }
    }
}

int main(int ac, char **av)
{
    // rendering(data);
    // return 0;

    std::signal(SIGINT, signalHandler);  // Ctrl + C
    std::signal(SIGTERM, signalHandler); // kill

    if (ac != 3)
    {
        cerr << "Usage: " << av[0] << " <ip> <port_num>" << endl;
        return 1;
    }

    string ip = av[1];
    int port_num = stoi(av[2]);

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        cerr << "Failed to create socket" << endl;
        return 1;
    }

    // Set up the server address
    // sockaddr_in serverAddr{}; C++11
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_num);
    if (inet_pton(AF_INET, ip.c_str(), &(serverAddr.sin_addr)) <= 0)
    {
        cerr << "Invalid address/Address not supported" << endl;
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cerr << "Connection failed" << endl;
        return 1;
    }

    for (int i = 0; i < MAX_THREAD; i++)
    {
        thread_arr[i] = nullptr;
    }

    thread_arr[0] = new std::thread(threadReceiveMessage, sockfd);
    thread_arr[1] = new std::thread(threadSocketNetwork, sockfd);

    while (1)
    {
        if (data.isGameStart == GAME_ING)
            break;
        std::cout << "Waiting for the game to start..." << std::endl;
    }

    rendering(data);

    cleanMemory();
    return 0;
}