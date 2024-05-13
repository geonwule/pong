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

#define MAX_THREAD_ 2
std::thread *thread_arr[MAX_THREAD_];
int sockfd;


#include "Util.hpp"
#include "Thread.hpp"
#include "Client.hpp"
std::atomic<bool> atom_stop(false);

int main(int ac, char **av)
{
    // rendering(data);
    // return 0;

    std::signal(SIGINT, signalHandler);  // Ctrl + C
    std::signal(SIGTERM, signalHandler); // kill

    if (!(ac == 2 || ac == 3))
    {
        std::cerr << "input:[ip] <port>" << std::endl;
        error_msg(WRONG_ARG);
    }


    if (ac == 2)
        Client::setInstance(av[1]);
    else // ac == 3
        Client::setInstance(av[1], av[2]);

    Client *client = Client::getInstance();
    if (client == nullptr)
        error_msg(FATAL_ERR);
    
    client->connectServer();
    client->handleServerCommunication();
    // return 0;

    while (1)
    {
        if (data.isGameStart == GAME_ING)
            break;
        // std::cout << "Waiting for the game to start..." << std::endl;
    }

    rendering(data);

    cleanMemory();
    return 0;
}