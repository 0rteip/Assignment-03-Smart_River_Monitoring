#include "UserConsole.h"
#include "Arduino.h"
#include "devices/ButtonImpl.h"
#include "devices/DisplayLcdI2C.h"
#include "kernel/MsgService.h"
#include <avr/sleep.h>
#include "config.h"

UserConsole::UserConsole()
{
  this->lcd = new DisplayLcdI2C();
  this->button = new ButtonImpl(BT_PIN);
  this->potentiometer = new Potentiometer(POT_PIN);
}

void UserConsole::init()
{
}

void UserConsole::sync()
{
  this->button->sync();
  this->potentiometer->sync();
}

bool UserConsole::isButtonPressed()
{
  return button->isPressed();
}

int UserConsole::getKnobValue()
{
  return this->potentiometer->getPotValue();
}

void UserConsole::turnOnDisplay()
{
  this->lcd->on();
}

void UserConsole::turnOffDisplay()
{
  this->lcd->off();
}

void UserConsole::displayModeDescription(String description)
{
  this->lcd->display(description);
}

void UserConsole::displayValveOpening()
{
  this->lcd->updateProgress(this->potentiometer->getPotValue());
}

void UserConsole::updateValveOpening(int value)
{
  this->lcd->updateProgress(value);
}

void UserConsole::sendValveMessage(int percentage)
{
  MsgService.sendMsg(VALVE_PREFIX + percentage);
}

void UserConsole::sendModeMessage(bool mode)
{
  MsgService.sendMsg(MODE_PREFIX + mode);
}

int UserConsole::newValveValue()
{
  if (MsgService.isMsgAvailable())
  {
    String message = MsgService.receiveMsg()->getContent();
    int prefixIndex = message.indexOf(VALVE_PREFIX);
    if (prefixIndex != -1)
    {
      String valueString = message.substring(prefixIndex + VALVE_PREFIX.length());
      return valueString.toInt();
    }
  }
  return -1;
}
