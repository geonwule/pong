#ifndef AGAMEELEMENT_HPP
#define AGAMEELEMENT_HPP

#pragma once

class AGameElement
{
private:
    AGameElement();

protected:
    float _x, _y;
    float _speed;

public:
    AGameElement(float x, float y, float speed);
    AGameElement(const AGameElement &other);
    AGameElement &operator=(const AGameElement &other);
    virtual ~AGameElement();
    virtual void move() = 0; // 순수 가상 함수
};

#endif