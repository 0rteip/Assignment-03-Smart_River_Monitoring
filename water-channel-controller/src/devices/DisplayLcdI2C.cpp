#include "DisplayLcdI2C.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

DisplayLcdI2C::DisplayLcdI2C()
{
    this->lcd = new LiquidCrystal_I2C(0x27, LCD_LENGHT, 2);
    this->lcd->init();
    this->lcd->backlight();
    this->lcd->leftToRight();

    this->lcd->createChar(0, this->zero);
    this->lcd->createChar(1, this->one);
    this->lcd->createChar(2, this->two);
    this->lcd->createChar(3, this->three);
    this->lcd->createChar(4, this->four);
    this->lcd->createChar(5, this->five);
}

void DisplayLcdI2C::on()
{
    this->lcd->backlight();
}

void DisplayLcdI2C::off()
{
    this->lcd->noBacklight();
}

void DisplayLcdI2C::clear()
{
    this->lcd->clear();
}

void DisplayLcdI2C::display(String message)
{
    this->clear();
    this->lcd->print(message);
    this->message = message;
}

void DisplayLcdI2C::updateProgress(int progress)
{
    // this->clear();
    // this->display(this->message);
    this->lcd->setCursor(0, 1);
    this->lcd->print("Opening: ");
    this->lcd->print(progress);
    this->lcd->print("%  ");
}
