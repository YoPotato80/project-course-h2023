#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4OLED oled;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;

int pos = 0;
unsigned int lineSensorValues[5];


void setup(){
    oled.init();
    lineSensors.initFiveSensors();
    oled.clear();
    oled.print(F("trykk"));
    oled.gotoXY(0, 1);
    oled.print(F("A"));

    Wire.begin();
    buttonA.waitForButton();
    oled.clear();
    oled.print(F("kalibrerer"));
    delay(1000);

    for (int i = 0; i < 60; i++){
        motors.setSpeeds(-200, 200);
        lineSensors.calibrate();
        delay(20);
    }
    motors.setSpeeds(0, 0);
    oled.clear();
}



void loop(){
    pos = lineSensors.readLine(lineSensorValues);
    oled.gotoXY(0, 0);
    oled.print(pos);
    delay(1);
    if (pos < 1500){
        motors.setSpeeds(0, 200);
    oled.print("😁");
    }else if(pos > 2500){
        motors.setSpeeds(200, 0);
    oled.print("😂");
    }else{
        motors.setSpeeds(400, 400);
    oled.print("😎");
    }

    delay(1);

}