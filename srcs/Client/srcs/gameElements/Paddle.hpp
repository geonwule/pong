#ifndef PADDLE_HPP
#define PADDLE_HPP

#include "AGameElement.hpp"
#include <algorithm>

#define PADDLE_WIDTH 0.1f
#define PADDLE_HEIGHT 0.3f
#define PADDLE_SPEED 0.02f

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
    // int player_area;
    Paddle();
public:
    Paddle(e_paddle player_num);
    Paddle(const Paddle &other);
    Paddle &operator=(const Paddle &other);
    ~Paddle();

    float getX() const { return _x; }
    float getY() const { return _y; }
    float getSpeed() const { return _speed; }
    float getWidth() const { return _width; }
    float getHeight() const { return _height; }
    e_paddle getDirection() const { return _direction; }

    void move() override
    {
        if (this->_direction == STOP)
            return;
        else if (this->_direction == UP)
            this->_y = std::max(this->_y + this->_speed, 1.0f - this->_height);
        else if (this->_direction == DOWN)
            this->_y = std::min(this->_y - this->_speed, -1.0f);
    }
    void setDirection(e_paddle direction)
    {
        this->_direction = direction;
    }
};

Paddle::Paddle(e_paddle player_num) : AGameElement(0.0f, 0.0f, PADDLE_SPEED), _width(PADDLE_WIDTH), _height(PADDLE_HEIGHT), _direction(STOP)
{
    if (player_num == PLAYER1)
        this->_x = -1.0f;
    else if (player_num == PLAYER2)
        this->_x = 1.0f - this->_width;
}

Paddle::Paddle(const Paddle &other) : AGameElement(other._x, other._y, other._speed), _width(other._width), _height(other._height), _direction(other._direction)
{
}

Paddle &Paddle::operator=(const Paddle &other)
{
    if (this != &other)
    {
        this->_x = other._x;
        this->_y = other._y;
        this->_speed = other._speed;
        this->_width = other._width;
        this->_height = other._height;
        this->_direction = other._direction;
    }
    return *this;
}

Paddle::~Paddle() {}

#endif