#include <atomic>
#include "Server.hpp"
#include "GameFrame.hpp"
#include "Thread.hpp"
#include "Util.hpp"

extern std::atomic<bool> atom_stop;

void threadReceiveData(s_Client &player, Paddle &paddle)
{
    Server *server = Server::getInstance();
    if (server == nullptr)
        error_msg(FATAL_ERR);

    std::string msg;
    while (!atom_stop)
    {
        server->receiveGameData(player);
        paddle.setDirection(player.msg);
    }
    std::cout << "threadReceiveData over..." << std::endl;
}

void threadSendData(GameData &data)
{
    Server *server = Server::getInstance();
    if (server == nullptr)
        error_msg(FATAL_ERR);
    
    while (data.isGameStart == GAME_LOADING)
    {
        server->sendGameData(GAME_LOADING, &data);
        sleep(1);
    }

    std::cout << "Game Start" << std::endl;
    server->sendGameData(GAME_START);

    while (!atom_stop)
    {
        server->sendGameData(GAME_ING, &data);
    }
    std::cout << "threadSendData over..." << std::endl;
}

static void inputData(GameData &data, Paddle &p1, Paddle &p2, CircleObject &ball)
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
    data.isGameStart = GAME_ING;
}

void playGame()
{
    GameData data;
    Server *server = Server::getInstance();
    if (server == nullptr)
        error_msg(FATAL_ERR);

    s_Client *player1, *player2;
    player1 = server->getClient();
    player2 = server->getClient();

    if (player1 == nullptr || player2 == nullptr)
        return ;

    Paddle p1(PLAYER1), p2(PLAYER2);
    CircleObject ball;
    inputData(data, p1, p2, ball);
    data.isGameStart = GAME_LOADING;

    Thread::createThread(threadSendData, std::ref(data));
    sleep(3); //loading time

    data.isGameStart = GAME_ING;
    
    sleep(1); //window loading time
    Thread::createThread(threadReceiveData, std::ref(*player1), std::ref(p1));
    Thread::createThread(threadReceiveData, std::ref(*player2), std::ref(p2));
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
    data.isGameStart = GAME_END;
    std::cout << "PlayGame over..." << std::endl;

}