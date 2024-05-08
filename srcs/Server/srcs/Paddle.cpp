#include "Paddle.hpp"

Paddle::Paddle(e_paddle player_num) : AGameElement(0.0f, 0.0f, PADDLE_SPEED), _width(PADDLE_WIDTH), _height(PADDLE_HEIGHT), _direction(STOP), _life(5)
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

int &Paddle::getLife() { return _life; }
float Paddle::getX() const { return _x; }
float Paddle::getY() const { return _y; }
float Paddle::getSpeed() const { return _speed; }
float Paddle::getWidth() const { return _width; }
float Paddle::getHeight() const { return _height; }
e_paddle Paddle::getDirection() const { return _direction; }

void Paddle::move()
{
    if (this->_direction == STOP)
        return;
    else if (this->_direction == UP)
        this->_y = std::max(this->_y + this->_speed, 1.0f - this->_height);
    else if (this->_direction == DOWN)
        this->_y = std::min(this->_y - this->_speed, -1.0f);
}
void Paddle::setDirection(e_paddle direction)
{
    this->_direction = direction;
}
