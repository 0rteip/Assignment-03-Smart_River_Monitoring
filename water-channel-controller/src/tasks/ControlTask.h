#ifndef __CONTROL_TASK__
#define __CONTROL_TASK__

#include "kernel/Task.h"
#include "UserConsole.h"
#include "model/WaterChannel.h"
#include "tasks/ValveControlTask.h"

class ControlTask : public Task
{

public:
    ControlTask(UserConsole *userConsole, WaterChannel *waterChannel, ValveControlTask *valveControlTask);
    void tick();

private:
    UserConsole *userConsole;
    WaterChannel *waterChannel;
    ValveControlTask *valveControlTask;
};

#endif
