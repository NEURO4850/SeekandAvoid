#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotor = AFMS.getStepper(360, 2);

const int pingPin = 7;

double angle, newAngle;
double x = 0;
double y = -1;
double angleDiff;
double counter = 0;
double backAngle;

void setup() {
  AFMS.begin();
  Serial.begin(9600);
  myMotor->setSpeed(80);

}

void loop() {

  //get pack from one group, if no obstructions forward to wheels,
  //else alter pack to avoid obstacle
  //y*=-1;
  //Serial.println(x);
  //Coord to angle
  newAngle = coorToAng(x, y);
  angleDiff = newAngle - counter;
  //Serial.println(newAngle);
  //get direction from other group
  //newAngle=40;
  // If result is larger then 360 then do nothing, or something they tell us not to move
  if (angleDiff > 0) {
    Serial.println("CounterClockwise");
    go(angleDiff, 1);
  } else if (angleDiff < 0) {
    Serial.println("Clockwise");
    go(angleDiff, 0);
  } else {
    idle();
    //Serial.println("idleing!");
    // If angle changed then check for obstacle in new direction
  }

  angle = newAngle;
  counter = newAngle;

}

// Scan the direction facing, return true if there is a obstacle
long getDist() {
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

  return cm;
}

void checkDir() {
  long cm = getDist();
  if (cm < 50) {
    x = 0;
    y = 0;
    Serial.println(cm);
    Serial.println("something in the way");
    myMotor->step(50, 0, DOUBLE);
    Serial.println("Clock 90");
    long sideCM = getDist();
    if (sideCM < 50) {
      Serial.println("something to the right");
      myMotor->step(100, 1, DOUBLE);
      Serial.println("Counter 180");
      sideCM = getDist();
      myMotor->step(50, 0, DOUBLE);
      Serial.println("Clock 90");
      if (sideCM < 50) {
        x *= -1;
        y *= -1;
        Serial.println("reverse");
      } else {
        angToCoor(90);
        Serial.println("left");
      }
    } else {
      angToCoor(-90);
      myMotor->step(50, 0, DOUBLE);
      Serial.println("Right");
    }
  }
}


void go(double angle, int direct) {
  angle = abs(angle);
  int steps = angle / 1.8;
  myMotor->step(steps, direct, DOUBLE);

  checkDir();
  delay(100);

}

void idle() {
  delay(100);
}

//Coord to angle
double coorToAng(double x, double y) {
  double ang = atan2(y, x);
  ang = ang * (180 / 3.14159265359);
  /* if (x < 0 && y > 0) {
     ang += 180;
    } else if (x < 0 && y < 0) {
     ang -= 180;
    } else {
     ang = ang;
    }
  */
  return ang;
}

//get Coord from angle
void angToCoor(int w) {
  y = tan(newAngle + w) * x;
}

