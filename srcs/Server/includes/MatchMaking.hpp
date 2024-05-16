#ifndef MATCHMAKING_HPP
#define MATCHMAKING_HPP

#include <iostream>
#include "Server.hpp"
#include "Thread.hpp"

class MatchMaking
{

private:
    static std::thread *_play_thread[MAX_THREAD];
    static size_t _num_players;
    static s_Client *_players[MAX_CLIENTS];
    static void matchPlayers();
    static bool checkDoublePlayer(s_Client *player);
    static void removePlayer(s_Client *player);

public:
    static void addPlayer(s_Client *player);
};

#endif // MATCHMAKING_HPP