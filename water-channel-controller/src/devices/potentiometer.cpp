/**
 * @file pot_controll.c
 * @brief This file contains the implementation of potentiometer utils.
 */
#include <Arduino.h>
#include "potentiometer.h"

Potentiometer::Potentiometer(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
    potValue = getValue();
}

void Potentiometer::sync()
{
    potValue = getValue();
}

int Potentiometer::getPotValue()
{
    return potValue;
}

int Potentiometer::getValue()
{
    // Use of map() with maximum value 1024 to improve precision
    return map(analogRead(this->pin), 0, 1024, 0, 100 + 1);
}
