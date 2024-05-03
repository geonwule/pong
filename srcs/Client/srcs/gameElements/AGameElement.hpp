#ifndef AGAMEELEMENT_HPP
#define AGAMEELEMENT_HPP

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

AGameElement::AGameElement(float x, float y, float speed) : _x(x), _y(y), _speed(speed)
{
}

AGameElement::AGameElement(const AGameElement &other) : _x(other._x), _y(other._y), _speed(other._speed)
{
}

AGameElement &AGameElement::operator=(const AGameElement &other)
{
    if (this != &other)
    {
        this->_x = other._x;
        this->_y = other._y;
        this->_speed = other._speed;
    }
    return *this;
}

AGameElement::~AGameElement() {}

#endif