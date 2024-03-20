#include "model/WaterChannel.h"
#include "config.h"
#include "devices/Valve.h"
#include <Arduino.h>

void wakeUp() {}

WaterChannel::WaterChannel(UserConsole *userConsole)
    : userConsole(userConsole)
{
    this->valve = new Valve(VALVE_PIN);
    this->changeMode(AUTOMATIC);
}

void WaterChannel::switchMode()
{
    if (this->isAutomaticMode())
    {
        this->changeMode(MANUAL);
    }
    else
    {
        this->changeMode(AUTOMATIC);
    }
}

String WaterChannel::getModeDescription()
{
    switch (this->mode)
    {
    case AUTOMATIC:
        return String("Automatic");
    case MANUAL:
        return String("Manual");
    default:
        return String();
    }
}

bool WaterChannel::isAutomaticMode()
{
    return this->mode == AUTOMATIC;
}

bool WaterChannel::isManualMode()
{
    return this->mode == MANUAL;
}

int WaterChannel::getValveOpening()
{
    return this->valve->getOpening();
}

void WaterChannel::readValveKnob()
{
    if (this->lastKnobValue == this->userConsole->getKnobValue())
    {
        return;
    }

    this->userConsole->displayValveOpening();
    this->lastKnobValue = this->userConsole->getKnobValue();
}

void WaterChannel::updateValveOpening()
{
    int potValue = this->userConsole->getKnobValue();
    this->valve->setOpening(potValue);
    this->userConsole->sendValveMessage(potValue);
}

void WaterChannel::changeMode(Mode mode)
{
    this->mode = mode;
    this->userConsole->displayModeDescription(this->getModeDescription());
    this->userConsole->sendModeMessage(this->isAutomaticMode());
}
