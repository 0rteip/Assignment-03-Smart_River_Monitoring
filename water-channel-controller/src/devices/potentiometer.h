/**
 * @file pot_controll.h
 * @brief This file contains the declaration of potentiometer utils.
 */
#ifndef __POT_CONTROLL__
#define __POT_CONTROLL__

#define DELAY_TIME 120

class Potentiometer
{
public:
    /**
     * Potentiometer initialization.
     */
    Potentiometer(int pin);

    void sync();

    /**
     * Get the potentiometer value.
     *
     * @return The potentiometer value from 1 to 4.
     */
    int getPotValue();

private:
    int pin;
    int potValue;
    int lastReadPotValue;

    int getValue();
};

#endif
