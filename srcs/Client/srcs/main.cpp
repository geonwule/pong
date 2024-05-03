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

void rendering();

void threadReceiveMessage(int sockfd)
{
    char buffer[1024] = {0};
    while (1)
    {
        int valread = recv(sockfd, buffer, 1024, 0);
        if (valread <= 0)
        {
            cerr << "Failed to receive message" << endl;
            exit(1);
            return;
        }
        buffer[valread] = 0;
        cout << buffer << endl;
    }
}

void threadSocketNetwork(int sockfd)
{
    while (1)
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
    // rendering();
    // return 0;
    
    if (ac != 3)
    {
        cerr << "Usage: " << av[0] << " <ip> <port_num>" << endl;
        return 1;
    }

    string ip = av[1];
    int port_num = stoi(av[2]);

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    std::thread t1(threadReceiveMessage, sockfd);
    std::thread t2(threadSocketNetwork, sockfd);

    rendering();

    t1.join();
    t2.join();
    close(sockfd);
    return 0;
}