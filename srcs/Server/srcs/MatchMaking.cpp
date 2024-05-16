#include "MatchMaking.hpp"
#include "Thread.hpp"
#include "Util.hpp"

size_t MatchMaking::_num_players = 0;
s_Client *MatchMaking::_players[MAX_CLIENTS] = {0};
void playGame(s_Client *player1,  s_Client *player2);

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
    
    Thread::createThread(playGame, player1, player2); //,check_thread_over); playGame이 종료되었음을 알수있는 변수를 넣고 그변수를 저장하고 있는 스레드에서 핸들링 하고 있다가 join해주게 추가 예정
    std::cout << "[MatchMaking::matchPlayers] over" << std::endl;
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