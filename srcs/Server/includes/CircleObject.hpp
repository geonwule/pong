#ifndef CIRCLEOBJECT_HPP
#define CIRCLEOBJECT_HPP

#include "AGameElement.hpp"
#include <cmath>   // sin, cos, M_PI(원주율 3.14....)
#include <cstdlib> //rand
#include <string>

#define CIRCLE_RADIUS 0.05f
#define CIRCLE_SPEED 0.01f

enum e_circle
{
    TO_RANDOM,
    TO_PLAYER1,
    TO_PLAYER2
};

// ball, item인스턴스
class CircleObject : public AGameElement
{

private:
    /* 부모 변수
    float x, y;
    float speed;*/
    float _dirX, _dirY;
    float _radius;

public:
    CircleObject();
    CircleObject(const CircleObject &other);
    CircleObject &operator=(const CircleObject &other);
    ~CircleObject();

    float getX() const { return _x; }
    float getY() const { return _y; }
    float getSpeed() const { return _speed; }
    float getDirX() const { return _dirX; }
    float getDirY() const { return _dirY; }
    float getRadius() const { return _radius; }

    void move() override
    {
        this->_x += this->_speed * this->_dirX;
        this->_y += this->_speed * this->_dirY;
    }

    void collisions(float paddle1Y, float paddle2Y)
    {
        if (this->_x <= -0.9f && this->_y >= paddle1Y && this->_y <= paddle1Y + 0.3f)
            this->_dirX = -this->_dirX; // 왼쪽 Paddle
        else if (this->_x >= 0.9f && this->_y >= paddle2Y && this->_y <= paddle2Y + 0.3f)
            this->_dirX = -this->_dirX; // 오른쪽 Paddle
        else if (this->_y <= -1.0f || this->_y >= 1.0f)
            this->_dirY = -this->_dirY; // 위쪽 또는 아래쪽 벽
    }

    void checkLives(int &player1Lives, int &player2Lives)
    {
        if (this->_x < -0.91f)
        {
            player1Lives--;
            this->_x = 0.0f;
            this->_y = 0.0f;
        }
        else if (this->_x > 0.91f)
        {
            player2Lives--;
            this->_x = 0.0f;
            this->_y = 0.0f;
        }
    }
    void get_random_direction(e_circle to);
};

CircleObject::CircleObject() : AGameElement(0.0f, 0.0f, CIRCLE_SPEED), _dirX(0.0f), _dirY(0.0f), _radius(CIRCLE_RADIUS)
{
    get_random_direction(TO_RANDOM);
}

CircleObject::CircleObject(const CircleObject &other) : AGameElement(other._x, other._y, other._speed), _dirX(other._dirX), _dirY(other._dirY), _radius(other._radius)
{
}

CircleObject &CircleObject::operator=(const CircleObject &other)
{
    if (this != &other)
    {
        this->_x = other._x;
        this->_y = other._y;
        this->_speed = other._speed;
        this->_dirX = other._dirX;
        this->_dirY = other._dirY;
        this->_radius = other._radius;
    }
    return *this;
}

CircleObject::~CircleObject() {}

void CircleObject::get_random_direction(e_circle to) //(e_circle to = TO_RANDOM)
{
    const double add_min_angle = -40 * M_PI / 180;
    const double min_angle = 20 * M_PI / 180;
    const double max_angle = 40 * M_PI / 180;

    double angle = min_angle + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max_angle - min_angle)));
    double add_angle = add_min_angle + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max_angle - add_min_angle)));

    if (to == TO_PLAYER1)
    {
        _dirX = -std::cos(add_angle);
        _dirY = std::sin(add_angle) * (static_cast<double>(rand()) / RAND_MAX < 0.5 ? 1 : -1);
    }
    else if (to == TO_PLAYER2)
    {
        _dirX = std::cos(add_angle);
        _dirY = std::sin(add_angle) * (static_cast<double>(rand()) / RAND_MAX < 0.5 ? 1 : -1);
    }
    else
    {
        _dirX = std::cos(angle) * (static_cast<double>(rand()) / RAND_MAX < 0.5 ? 1 : -1);
        _dirY = std::sin(angle) * (static_cast<double>(rand()) / RAND_MAX < 0.5 ? 1 : -1);
    }
}

#endif