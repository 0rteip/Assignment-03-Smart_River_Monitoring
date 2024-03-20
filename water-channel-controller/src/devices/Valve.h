#ifndef __VALVE__
#define __VALVE__

#include "ServoMotorImpl.h"

#define OPEN_ANGLE 2250
#define CLOSE_ANGLE 1570

class Valve : public ServoMotorImpl
{

public:
    Valve(int pin);

    void open();
    void close();

    void setOpening(int percentage);
    int getOpening();

private:
    int getAngle(int percentage);
    int opening;
};

#endif
