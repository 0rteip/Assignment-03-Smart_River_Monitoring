#include "Valve.h"

Valve::Valve(int pin) : ServoMotorImpl(pin)
{
    this->close();
}

void Valve::open()
{
    this->on();
    this->setPosition(OPEN_ANGLE);
    this->opening = OPEN_ANGLE;
    this->off();
}

void Valve::close()
{
    this->on();
    this->setPosition(CLOSE_ANGLE);
    this->opening = CLOSE_ANGLE;
    this->off();
}

void Valve::setOpening(int percentage)
{
    this->on();
    this->opening = percentage;
    this->setPosition(this->getAngle(percentage));
    this->off();
}

int Valve::getOpening()
{
    return this->opening;
}

int Valve::getAngle(int percentage)
{
    return map(percentage, 0, 100, CLOSE_ANGLE, OPEN_ANGLE);
}
