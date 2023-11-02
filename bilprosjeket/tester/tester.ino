#include <Wire.h>
#include <Zumo32U4.h>
#include <Zumo32U4OLED.h> // Added library for OLED

Zumo32U4Motors motors;
Zumo32U4OLED oled; // Changed object to OLED
Zumo32U4ButtonA buttonA;

void setup()
{
    oled.init(); // initialize the OLED screen
    oled.clear(); // Changed object to OLED
    oled.print(F("Calibrating"));
    oled.gotoXY(0, 1);
    oled.print(F("sensors..."));

    Wire.begin();
    delay(1000);

    buttonA.waitForButton();
    
    oled.clear();
    oled.print(F("Place on"));
    oled.gotoXY(0, 1);
    oled.print(F("flat surface"));
    buttonA.waitForButton();

    oled.clear();
    oled.print(F("Calibrating"));
    oled.gotoXY(0, 1);
    oled.print(F("gyro..."));
    delay(1000);
    Zumo32U4IMU imu;
    imu.init();
    oled.clear();
    oled.print(F("Done!"));
    delay(1000);
}

void loop()
{
    // Your code here
}
