#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(int ac, char **av)
{
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
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cerr << "Connection failed" << endl;
        return 1;
    }
    while (1)
    {
        string message;
        cout << "Enter message: ";
        getline(cin, message);

        if (message == "exit")
            break;

        if (send(sockfd, message.c_str(), message.size(), 0) == -1)
        {
            cerr << "Failed to send message" << endl;
            return 1;
        }
    }

    close(sockfd);
    return 0;
}