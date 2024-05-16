#include "Server.hpp"
#include "GameFrame.hpp"
#include "Thread.hpp"
#include "Util.hpp"
#include "Cache.hpp"

void threadReceiveData(GameData &data, s_Client &player, Paddle &paddle)
{
    std::cout << "[threadReceiveData] start..." << std::endl;
    Server *server = Server::getInstance();
    if (server == nullptr)
        error_msg(FATAL_ERR);

    std::string msg;
    while (!Cache::atom_stop)
    {
        if (data.isGameStart == GAME_END)
            break;
        if (server->receiveGameData(player, data.isGameStart) == EXIT_SUCCESS)
            paddle.setDirection(player.msg);
    }
    std::cout << "[threadReceiveData] over..." << std::endl;
}

void threadSendData(GameData &data, int *players_id)
{
    Server *server = Server::getInstance();
    if (server == nullptr)
        error_msg(FATAL_ERR);
    
    while (data.isGameStart == GAME_LOADING)
    {
        server->sendGameData(GAME_LOADING, players_id, &data);
        sleep(1);
    }

    std::cout << "[threadSendData] Game Start" << std::endl;
    server->sendGameData(GAME_START, players_id);

    while (!Cache::atom_stop)
    {
        if (data.isGameStart == GAME_END)
            break;
        server->sendGameData(GAME_ING, players_id, &data);
    }
    server->sendGameData(GAME_END, players_id, &data);
    std::cout << "[threadSendData] over..." << std::endl;
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

void playGame(s_Client *player1,  s_Client *player2)
{
    std::cout << "[playGame] start...\n";
    if (player1 == nullptr || player2 == nullptr)
        error_msg(FATAL_ERR);

    player1->is_game_ing = true;
    player2->is_game_ing = true;

    GameData data;
    Server *server = Server::getInstance();
    if (server == nullptr)
        error_msg(FATAL_ERR);

    // s_Client *player1, *player2;
    // player1 = server->getPlayer();
    // player2 = server->getPlayer();

    int players_id[2] = {player1->id, player2->id};


    Paddle p1(PLAYER1), p2(PLAYER2);
    CircleObject ball;
    inputData(data, p1, p2, ball);
    data.isGameStart = GAME_LOADING;

    std::thread *t[3];

    t[0] = Thread::createThread(threadSendData, std::ref(data), players_id);
    sleep(3); //loading time

    data.isGameStart = GAME_ING;
    
    sleep(1); //window loading time
    t[1] = Thread::createThread(threadReceiveData, std::ref(data), std::ref(*player1), std::ref(p1));
    t[2] = Thread::createThread(threadReceiveData, std::ref(data), std::ref(*player2), std::ref(p2));
    while (!Cache::atom_stop)
    {
        if (p1.getLife() == 0 || p2.getLife() == 0)
        {
            data.isGameStart = GAME_END;
            break;
        }
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

    for (int i = 0; i < 3; i++)
    {
        Thread::joinThread(t[i]);
    }

    player1->is_game_ing = false;
    player2->is_game_ing = false;
    std::cout << "[playGame] over..." << std::endl;
}