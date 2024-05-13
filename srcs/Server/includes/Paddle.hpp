#ifndef PADDLE_HPP
#define PADDLE_HPP

#pragma once

#include "AGameElement.hpp"
#include <algorithm>
#include <iostream>
#include <string>

#define PADDLE_WIDTH 0.1f
#define PADDLE_HEIGHT 0.3f
#define PADDLE_SPEED 0.01f

enum e_paddle
{
    UP,
    DOWN,
    STOP,
    PLAYER1,
    PLAYER2
};

class Paddle : public AGameElement
{
private:
    /* 부모 변수
    float x, y;
    float speed;*/
    float _width, _height;
    e_paddle _direction;
    int _life;
    // int player_area;
    Paddle();
public:
    Paddle(e_paddle player_num);
    Paddle(const Paddle &other);
    Paddle &operator=(const Paddle &other);
    ~Paddle();

    int &getLife();
    float getX() const;
    float getY() const;
    float getSpeed() const;
    float getWidth() const;
    float getHeight() const;
    e_paddle getDirection() const;

    void move() override;
    void setDirection(const std::string &direction);
};


#endif