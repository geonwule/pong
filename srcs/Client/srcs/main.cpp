#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <future>
#include <thread>
#include "../../mac_opengl/include/GLFW/glfw3.h"

using namespace std;

void func(int sockfd)
{
    char buffer[1024] = {0};
    while (1)
    {
        int valread = recv(sockfd, buffer, 1024, 0);
        if (valread == -1)
        {
            cerr << "Failed to receive message" << endl;
            return;
        }
        buffer[valread] = 0;
        cout << buffer << endl;
    }
}

void funcGlfw()
{
    if (!glfwInit()) {
        return;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return ;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return ;
}

void func2(int sockfd)
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

    std::thread t1(func, sockfd);
    std::thread t2(func2, sockfd);
    funcGlfw();
    // std::thread t2(funcGlfw);

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
            return 1;
        }
        else
        {
            cout << "Message sent: " << message << endl;
        }
    }
    // t1.join();
    // t2.join();
    close(sockfd);
    return 0;
}