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

    float getX() const;
    float getY() const;
    float getSpeed() const;
    float getDirX() const;
    float getDirY() const;
    float getRadius() const;

    void move() override;
    void collisions(float paddle1Y, float paddle2Y);
    void checkLives(int &player1Lives, int &player2Lives);
    void get_random_direction(e_circle to);
};

#endif