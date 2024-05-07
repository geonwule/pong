#ifndef GAMEFRAME_HPP
#define GAMEFRAME_HPP

#include "Paddle.hpp"
#include "CircleObject.hpp"

struct GameData {
    float ball_x;
    float ball_y;
    float ball_radius;

    float paddle_width;
    float paddle_height;
    float paddle1_x;
    float paddle1_y;
    float paddle2_x;
    float paddle2_y;
    
    int player1Lives;
    int player2Lives;

    int isGameStart;
};

#endif // GAMEFRAME_HPP