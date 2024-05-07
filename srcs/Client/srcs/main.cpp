#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <future>
#include <thread>
#include "../../../mac_opengl/include/GLFW/glfw3.h"

#define WIDTH 2000
#define HEIGHT 1000

using namespace std;
#include <sstream>
#include <cmath>
#include "GameFrame.hpp"

#include <csignal>
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
}

std::atomic<bool> atom_stop(false);

void signalHandler(int signum) {
    if (signum == SIGINT)
        std::cout << "SIGINT: Interrupt signal received" << std::endl;
    else if (signum == SIGTERM)
        std::cout << "SIGTERM: Termination signal received" << std::endl;
    else
        std::cout << "Others: Signal " << signum << " received" << std::endl;
    atom_stop = true;
    cleanMemory();
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
                cleanMemory();
                exit(1);
                return;
            }
            buffer[valread] = 0;
            cout << buffer << endl;
            if (string(buffer) == "Game Start")
                isGameStart = true;
        }
        else
        {
            int valread = recv(sockfd, &data, sizeof(data), 0);
            if (valread != sizeof(data)) //(valread <= 0)
            {
                cerr << "Failed to receive GameData" << endl;
                cleanMemory();
                exit(1);
                return;
            }
            std::cout << "Received GameData" << std::endl;
        }
    }
}

void threadSocketNetwork(int sockfd)
{
    while (!atom_stop)
    {
        string message;
        cout << "Enter message: ";
        getline(cin, message);
        message += '\n';

        if (message == "exit")
            break;

        if (send(sockfd, message.c_str(), message.size(), 0) == -1)
        {
            cerr << "Failed to send message" << endl;
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

    std::signal(SIGINT, signalHandler); // Ctrl + C
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
        if (data.isGameStart)
            break;
        std::cout << "Waiting for the game to start..." << std::endl;
        sleep(3);
    }

    rendering(data);

    for (int i = 0; i < MAX_THREAD; i++)
    {
        thread_arr[i]->join();
        delete thread_arr[i];
    }
    close(sockfd);
    return 0;
}