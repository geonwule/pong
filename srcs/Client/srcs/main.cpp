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

void singlePlay()
{
    if (!glfwInit())
    {
        return;
    }

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "pong", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    unsigned int i = 0;
    double t = 0.0;
    double dt = 0.01;
    while (!glfwWindowShouldClose(window))
    {

        std::cout << "loop: " << ++i << std::endl;
        glClear(GL_COLOR_BUFFER_BIT);

        std::stringstream ss;
        ss << "loop: " << i;
        glfwSetWindowTitle(window, ss.str().c_str());

        // Calculate circle position
        float x = 0.5f * std::cos(t);
        float y = 0.5f * std::sin(2 * t);

        glColor3f(1.0f, 1.0f, 1.0f);

        // Draw a circle at (x, y)
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 100; i++)
        {
            float angle = i * 2.0f * M_PI / 100.0f;
            glVertex2f(x + std::cos(angle) * 0.1f, y + std::sin(angle) * 0.1f);
        }

        float angle = i * 2.0f * M_PI / 100.0f;
        glVertex2f(x + std::cos(angle) * 0.1f, y + std::sin(angle) * 0.1f);

        glEnd();
        t += dt;
        // // Draw a bar that changes with i
        // float barWidth = (float)i / 100.0f; // Adjust this to change the speed of the bar
        // glBegin(GL_QUADS);
        // glVertex2f(-1.0f, -1.0f);
        // glVertex2f(-1.0f + barWidth, -1.0f);
        // glVertex2f(-1.0f + barWidth, 1.0f);
        // glVertex2f(-1.0f, 1.0f);
        // glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();

        usleep(1000);
    }

    glfwTerminate();
    return;
}

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
    if (!glfwInit())
    {
        return;
    }

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return;
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
    singlePlay();
    return 0; // test

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