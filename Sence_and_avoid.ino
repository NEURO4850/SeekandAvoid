#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotor = AFMS.getStepper(360, 2);

const int pingPin = 7;

int angle, newAngle;
int maxRot=180;
double x,y;

void setup() {
  AFMS.begin();
  Serial.begin(9600);
  myMotor->setSpeed(80);
}

void loop() {
  
//get pack from one group, if no obstructions forward to wheels, 
//else alter pack to avoid obstacle
x=-2;
y=2;
//Coord to angle
newAngle=coorToAng(x, y);
  Serial.println(newAngle);
  //get direction from other group
  //newAngle=40;
  // If result is larger then 360 then do nothing, or something they tell us not to move
  if(newAngle >360){
    idle();
  // If no new angle input then do nothing  
  }else if (angle == newAngle) {
    idle();
    Serial.println("idleing!");
  // If angle changed then check for obstacle in new direction 
  } else {
    Serial.println("start loop");
    angle = newAngle;

    go (angle);
    Serial.println("Going!");
  }

}

// Scan the direction facing, return true if there is a obstacle
bool checkDir() {
  long duration;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  long cm = duration / 29 / 2;

  if (cm < 50) {
    return true;
  }
  return false;
}


bool go(int angle) {
  myMotor->step(angle, FORWARD, DOUBLE);
  
  if(checkDir()){
    // Something in the way, alter direction
  }
  delay(100);
  return true;
}

void idle(){ 
  delay(100);
}

//Coord to angle
double coorToAng(double x, double y){
  double ang = atan2(y,x);
  ang=ang*(180/3.14159265359);
  /*if(x>0&&y<0){
    ang+=360;
  }else if(x<0&&y>0){
    ang+=180;
  }else if(x<0 && y<0){
    ang+=180
  }else{
    ang=ang;
  }*/
  return ang;
}

