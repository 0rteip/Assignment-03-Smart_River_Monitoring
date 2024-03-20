#ifndef __VALVE_CONTROL_TASK__
#define __VALVE_CONTROL_TASK__

#include "kernel/Task.h"
#include "UserConsole.h"
#include "model/WaterChannel.h"

#define VALVE_STABLE_TIME 200

class ValveControlTask : public Task
{

public:
    ValveControlTask(UserConsole *userConsole, WaterChannel *waterChannel);
    void tick();

private:
    UserConsole *userConsole;
    WaterChannel *waterChannel;

    unsigned long lastValveChange = 0;
    int lastKnobValue = 0;
};

#endif
