#include "Cache.hpp"

GameData initData = {
    0.5f, 0.5f, 0.02f,
    0.02f, 0.2f, 0.9f, 0.5f, 0.1f, 0.5f,
    3, 3,
    0
};

GameData Cache::data = initData;
std::atomic<bool> Cache::atom_stop(false);
bool Cache::isGameStart = false;

void Cache::resetData()
{
    data = initData;
}