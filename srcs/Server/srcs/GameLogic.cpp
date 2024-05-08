#include "Socket.hpp"
#include "Server.hpp"
#include "GameFrame.hpp"
#include <atomic>
#include "Thread.hpp"

extern std::atomic<bool> atom_stop;

void threadSendData(GameData &data)
{
    Server *server = Server::getInstance();
    if (server == nullptr)
    {
        std::cerr << "Server::getInstance() failed" << std::endl;
        return;
    }
    while (!atom_stop)
    {
        server->sendGameData("", data);
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
    Server *server = Server::getInstance();
    if (server == nullptr)
    {
        std::cerr << "Server::getInstance() failed" << std::endl;
        return;
    }
    server->sendGameData("Game Start", data);

    std::cout << "Game Start" << std::endl;
    Thread::createThread(threadSendData, std::ref(data));

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

}