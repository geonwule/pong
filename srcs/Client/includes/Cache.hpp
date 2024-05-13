#ifndef CACHE_HPP
#define CACHE_HPP

#include <atomic>
#include "GameFrame.hpp"

namespace Cache{
    extern GameData data;
    extern std::atomic<bool> atom_stop;
    extern bool isGameStart;
    void resetData();
}

#endif