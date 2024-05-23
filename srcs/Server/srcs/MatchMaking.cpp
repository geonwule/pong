#include "MatchMaking.hpp"
#include "Util.hpp"
#include "Cache.hpp"

std::thread *MatchMaking::_play_thread[MAX_THREAD] = {nullptr};
size_t MatchMaking::_num_players = 0;
s_Client *MatchMaking::_players[MAX_CLIENTS] = {0};
void playGame(s_Client *player1, s_Client *player2);

int MatchMaking::addPlayThread(std::thread *thread)
{
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (_play_thread[i] == nullptr)
        {
            _play_thread[i] = thread;
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

void MatchMaking::matchPlayers()
{
    std::cout << "[MatchMaking::matchPlayers] start" << std::endl;
    s_Client *player1 = nullptr, *player2 = nullptr;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (player1 && player2)
            break;

        if (_players[i] != nullptr)
        {
            if (player1 == nullptr)
                player1 = _players[i];
            else
                player2 = _players[i];
            removePlayer(_players[i]);
            _num_players--;
        }
    }

    if (player1 == nullptr || player2 == nullptr)
        error_msg(FATAL_ERR);

    std::thread *thread = Thread::createThread(playGame, player1, player2);
    if (addPlayThread(thread))
        error_msg(FATAL_ERR); // 스레드 추가 실패시 에러처리 추가 예정

    std::cout << "[MatchMaking::matchPlayers] over" << std::endl;
}

void MatchMaking::monitoringGameplayThread()
{
    size_t log_num = 0;
    while(!Cache::atom_stop)
    {
        log_num++;
        int cnt = 0;
        for (int i = 0; i < MAX_THREAD; i++)
        {
            if (_play_thread[i] != nullptr)
            {
                cnt++;
                Thread::joinThread(_play_thread[i]);
                _play_thread[i] = nullptr;
            }
        }
        sleep(1);
        if (cnt)
            std::cout << "monitoringGameplayThread...[" << cnt << "] game thread join" << std::endl; //test
        else if (log_num % 10 == 0)
            std::cout << "monitoringGameplayThread...no game thread join" << std::endl; //test
    }
}

bool MatchMaking::checkDoublePlayer(s_Client *player)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (_players[i] == player)
            return true;
    }
    return false;
}

void MatchMaking::addPlayer(s_Client *player)
{
    if (player == nullptr || checkDoublePlayer(player))
        return;

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (_players[i] == nullptr)
        {
            _players[i] = player;
            break;
        }
    }

    _num_players++;
    if (_num_players >= 2)
        matchPlayers();
}

void MatchMaking::removePlayer(s_Client *player)
{
    if (player == nullptr)
    {
        std::cerr << "[MatchMaking::removePlayer] Player is nullptr" << std::endl;
        return;
    }

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (_players[i] == player)
        {
            _players[i] = nullptr;
            break;
        }
    }
}