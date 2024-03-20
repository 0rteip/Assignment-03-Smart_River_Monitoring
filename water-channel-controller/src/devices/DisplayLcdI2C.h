#ifndef __DISPLAY_LCD_I2C__
#define __DISPLAY_LCD_I2C__

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#define LCD_LENGHT 16

class DisplayLcdI2C
{
public:
    DisplayLcdI2C();
    void on();
    void off();
    void clear();
    void display(String message);
    void updateProgress(int progress);

private:
    LiquidCrystal_I2C *lcd;
    String message;
};

#endif
