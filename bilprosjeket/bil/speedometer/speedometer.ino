#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4Motors motors;
Zumo32U4OLED oled;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;

int pos = 0;
int left = 0;
int right = 0;
unsigned int lineSensorValues[5];
int time = 50;
float avstand = 0;
float avstandtot = 0;
float distance[5];
float speed[5];
float fart = 0;
long lastmillis = 0;

void setup(){
    init();
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
    /*
    for (int i = 0; i < 60; i++){
        motors.setSpeeds(-200, 200);
        lineSensors.calibrate();
        delay(20);
    }
    */
    motors.setSpeeds(00, 00);
    oled.clear();
}

void speedcalc(){
    oled.clear();
    avstand = (0.12*(encoders.getCountsAndResetLeft() + encoders.getCountsAndResetRight()))/(2*12*75);  //12 counts per motor-rotasjon, 75 motorrotasjoner per hjul snurring hjul omkrets 12 cm
    avstandtot += avstand;
    fart = (avstand*1000)/((millis() - lastmillis));
    oled.gotoXY(0, 0);
    oled.print(fart);
    oled.gotoXY(0, 1);
    oled.print((avstandtot));
    
        lastmillis = millis();
}

void loop(){
    if (millis() - lastmillis > time){
        speedcalc();
    }
    /*
    pos = lineSensors.readLine(lineSensorValues);
    delay(1);
    
    if (pos < 1500){
        motors.setSpeeds(0, 200);
    }else if(pos > 2500){
        motors.setSpeeds(200, 0);
    }else{
        motors.setSpeeds(400, 400);
    }
    */

}