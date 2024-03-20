#include "Arduino.h"
#include "config.h"
#include "tasks/ValveControlTask.h"
#include "UserConsole.h"

ValveControlTask::ValveControlTask(UserConsole *userConsole, WaterChannel *waterChannel)
    : userConsole(userConsole), waterChannel(waterChannel)
{
}

void ValveControlTask::tick()
{

    unsigned long now = millis();

    this->waterChannel->readValveKnob();

    if (this->userConsole->getKnobValue() != this->lastKnobValue)
    {
        this->lastValveChange = now;
        this->lastKnobValue = this->userConsole->getKnobValue();
        return;
    }

    if (now - this->lastValveChange > VALVE_STABLE_TIME &&
        this->waterChannel->getValveOpening() != this->lastKnobValue)
    {
        this->lastValveChange = now;
        this->lastKnobValue = this->userConsole->getKnobValue();
        this->waterChannel->updateValveOpening();
    }
}
