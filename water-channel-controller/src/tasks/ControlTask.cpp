#include "Arduino.h"
#include "config.h"
#include "tasks/ControlTask.h"
#include "UserConsole.h"

ControlTask::ControlTask(UserConsole *userConsole, WaterChannel *waterChannel, ValveControlTask *valveControlTask)
    : userConsole(userConsole), waterChannel(waterChannel), valveControlTask(valveControlTask)
{
    this->userConsole->displayValveOpening();
}

void ControlTask::tick()
{
    // Sync the changes in the user console, for example, if the button is pressed
    this->userConsole->sync();
    if (this->userConsole->isButtonPressed())
    {
        this->waterChannel->switchMode();
        this->userConsole->displayValveOpening();
        this->valveControlTask->setActive(this->waterChannel->isManualMode());
    }
}
