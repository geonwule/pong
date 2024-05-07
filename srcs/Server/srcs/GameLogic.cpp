#include "Socket.hpp"
#include "GameFrame.hpp"
#include <atomic>

extern s_Client clients[MAX_CLIENTS];
extern std::atomic<bool> atom_stop;
#define MAX_THREAD 2
extern std::thread *thread_arr[MAX_THREAD];

void sendGameData(const std::string &msg, GameData &data)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        s_Client &client = clients[i];
        if (client.fd > 0)
        {
            ssize_t bytes_sent;
            if (msg == "")
            {
                bytes_sent = send(client.fd, &data, sizeof(data), 0);
                std::cout << "sendGameData" << std::endl;
            }
            else
                bytes_sent = send(client.fd, msg.c_str(), msg.size(), 0);
            if (bytes_sent == -1)
            {
                std::cerr << "Failed to send message to client " << client.id << std::endl;
            }
        }
    }
}

#include <thread>
std::thread *thread_sendData = nullptr;

void threadSendData(GameData &data)
{
    while (!atom_stop)
    {
        sendGameData("", data);
    }
    std::cout << "threadSendData over..." << std::endl;
}

void inputData(GameData &data, Paddle &p1, Paddle &p2, CircleObject &ball)
{
    data.ball_x = ball.getX();
    data.ball_y = ball.getY();
    data.ball_radius = ball.getRadius();

    data.paddle_width = p1.getWidth();
    data.paddle_height = p1.getHeight();
    data.paddle1_x = p1.getX();
    data.paddle1_y = p1.getY();
    data.paddle2_x = p2.getX();
    data.paddle2_y = p2.getY();

    data.player1Lives = p1.getLife();
    data.player2Lives = p2.getLife();

    data.isGameStart = 1;
}

void playGame()
{
    GameData data;
    sendGameData("Game Start", data);
    std::cout << "Game Start" << std::endl;
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (thread_arr[i] == nullptr)
        {
            thread_arr[i] = new std::thread(threadSendData, std::ref(data));       
            break;
        }
    }

    Paddle p1(PLAYER1), p2(PLAYER2);
    CircleObject ball;

    while (!atom_stop)
    {
        // Paddle
        p1.move();
        p2.move();

        // Ball
        ball.move();
        ball.collisions(p1.getY(), p2.getY());
        ball.checkLives(p1.getLife(), p2.getLife());

        inputData(data, p1, p2, ball);
        usleep(8000);
    }

    std::cout << "PlayGame over..." << std::endl;
    // if (thread_sendData != nullptr)
    // {
    //     thread_sendData->join();
    //     delete thread_sendData;
    //     thread_sendData = nullptr;
    // }
}