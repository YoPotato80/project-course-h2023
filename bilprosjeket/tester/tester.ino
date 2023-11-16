#include <arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

//Endre navn på bibliotek 
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4Encoders encoders;
Zumo32U4OLED display; 


//Variabler 
int16_t lastError = 0;
uint16_t maxSpeed = 300;                    //Hastighet 
float totalDistance = 0.0;                  //Total distansen til bilen 
#define NUM_SENSORS 5                       //Antall IR sensorer
unsigned int lineSensorValues[NUM_SENSORS]; //Verdien av avlesningen til sensorene
unsigned long lastDisplayTime = millis();   //Nåværende tidspunkt

//Array
int speedArray[60];
int arrayCounter = 0;

//Funksjonsliste

float averageSpeed() //Beregne gjennomsnittet av hastighetene i speedArray
{
  float sum = 0;
  for (int i = 0; i < 60; i++){
    sum += speedArray[i];
  }
  return sum/ 60.0;
}

int findMaxSpeed() //Finner makshastigheten i speedArray
{
  int maxSpeed = speedArray[0];
  for (int i = 1; i < 60; i++){
    if (speedArray[i] > maxSpeed){
      maxSpeed = speedArray[i];
    }
  }
  return maxSpeed;
} 

int over70Per(int absMaxSpeed)//Tell ant. sekunder bilen har kjørt raskere enn 70%
{
  int count = 0;
  for (int i=0; i<60; i++){
    if (speedArray[i] > 0.7*absMaxSpeed){
      count +=1;
    }
  }
  return count;
}

void calibrateSensors()
{
  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    
    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void lineFollower()
{
  int16_t position = lineSensors.readLine(lineSensorValues);

  //Beregne hvor langt unna linjen er senteret
  int16_t error = position - 2000; 
  int16_t speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;   //Deklarerer nytt navn for error

  //Sett farten til de individuelle motorene 
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;

  //Sett grenser til farten til motoren fra 0 - maxspeed (200)
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);

  //Sett farten til de to motorene 
  motors.setSpeeds(leftSpeed, rightSpeed);
}

void speedOmeter() {
  //Tidskontroll
  Serial.print("Hallo");
  if ((millis() - lastDisplayTime) >= 1000) {  
    lastDisplayTime = millis();//Oppdatering av tidspunktet
    Serial.println("Hei");

    //Hent encoderavlseninger
    int16_t countsLeft = encoders.getCountsAndResetLeft();
    int16_t countsRight = encoders.getCountsAndResetRight();
    
    //Gjennomsnitt av enoderpulsinger
    int count = (countsLeft + countsRight) / 2; // Gjennomsnittet av encoderen av begge motorene

    //Konverter til lengde og hastighet
    int length = (count / 909.7) * 10.99; //cm
    int speed = length / 0.1;// cm/s

    //Oppdater total distanse
    totalDistance = totalDistance + length;  

    //Vis på OLED-display 
    display.clear();
    display.gotoXY(0, 0);
    display.print("S:");
    display.print(speed);

    display.gotoXY(0, 1);
    display.print("D:");
    display.print(totalDistance);

   if(arrayCounter < 60)
    {
      speedArray[arrayCounter] = speed;
      arrayCounter += 1;
    }
    else
    {
      arrayCounter = 0;
      //Gjennomsnittshastigheten
      float avgSpeed = averageSpeed();
      //Makshastigheten
      int absMaxSpeed = findMaxSpeed();
      //Raskere enn 70%
      int seconds70Per = over70Per(absMaxSpeed);
      display.clear();
      display.gotoXY(0, 2);
      display.print("A");
      display.print(avgSpeed);
    } 
  }
}


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  lineSensors.initFiveSensors();
  buttonA.waitForButton();
  calibrateSensors();
  
}

void loop()
 {
  
  //lineFollower();
  motors.setSpeeds(50,50);
  speedOmeter();

}