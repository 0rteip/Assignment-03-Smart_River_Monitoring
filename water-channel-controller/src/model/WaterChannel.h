#ifndef __WATER_CHANNEL__
#define __WATER_CHANNEL__

#include <Arduino.h>
#include "UserConsole.h"
#include "devices/Valve.h"
#include "config.h"
#include "WaterChannelMode.h"

class WaterChannel
{
public:
    WaterChannel(UserConsole *userConsole);

    // set mode
    void switchMode();
    String getModeDescription();

    // get mode
    bool isAutomaticMode();
    bool isManualMode();

    int getValveOpening();

    void readValveKnob();
    void updateValveOpening();
    void setValveOpening(int value);

private:
    void changeMode(Mode mode);

    int lastKnobValue = -1;

    Mode mode;
    UserConsole *userConsole;
    Valve *valve;
};

#endif
