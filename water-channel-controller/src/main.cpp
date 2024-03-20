#include <Arduino.h>
#include "kernel/Scheduler.h"
#include "model/WaterChannel.h"
#include "tasks/ControlTask.h"
#include "tasks/ValveControlTask.h"
#include "UserConsole.h"

Scheduler *sched;
WaterChannel *waterChannel;
UserConsole *userConsole;

void setup()
{
  sched = new Scheduler();
  sched->init(25);

  userConsole = new UserConsole();
  waterChannel = new WaterChannel(userConsole);

  ValveControlTask *valveControlTask = new ValveControlTask(userConsole, waterChannel);
  valveControlTask->init(200);
  valveControlTask->setActive(false);

  ControlTask *controlTask = new ControlTask(userConsole, waterChannel, valveControlTask);
  controlTask->init(125);
  // controlTask->setActive(false);

  sched->addTask(controlTask);
  sched->addTask(valveControlTask);

  Serial.begin(115200);
}

void loop()
{
  sched->schedule();
}
