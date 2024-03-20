#ifndef __USER_CONSOLE__
#define __USER_CONSOLE__

#include <Wire.h>
#include "config.h"
#include "devices/Button.h"
#include "devices/DisplayLcdI2C.h"
#include "devices/potentiometer.h"

#define MODE_PREFIX String("md:")
#define VALVE_PREFIX String("vl:")

class UserConsole
{

public:
  UserConsole();

  void init();
  void sync();

  void turnOnDisplay();
  void turnOffDisplay();

  void displayModeDescription(String description);

  void displayValveOpening();
  void updateValveOpening(int value);

  void sendModeMessage(bool mode);
  void sendValveMessage(int percentage);

  bool isButtonPressed();
  int getKnobValue();

  int newValveValue();

private:
  Button *button;
  DisplayLcdI2C *lcd;
  Potentiometer *potentiometer;
};

#endif
