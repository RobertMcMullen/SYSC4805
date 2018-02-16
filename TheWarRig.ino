#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(4);

int Left = LOW;
int FarLeft = LOW;
int FarRight = LOW;
int Right = LOW;
int Center = LOW;

//New IR Sensor Module
//       _____________________
//      /     B     A    C     \
//    /  D                  E   \
//   |                          |
//   |                          |
//   ----------------------------

const int pin12D = 2;
const int pin11B = 3; //middle left
const int pin4A = 4; //middle sensor
const int pin3C = 5;
const int pin2E = 6;


void setup() {
Serial.begin(9600);
AFMS.begin();
pinMode(pin12D, INPUT);
pinMode(pin11B, INPUT);
pinMode(pin4A, INPUT);
pinMode(pin3C, INPUT);
pinMode(pin2E, INPUT);

}


void loop() {
//FarLeft = digitalRead(pin12D);
Left = digitalRead(pin11B);
Center = digitalRead(pin4A);
Right = digitalRead(pin3C);
//FarRight = digitalRead(pin2E);

Drive();

if(Left == LOW){
  Brake();
  ShiftLeft();
}

if(Right == LOW){
  Brake();
  ShiftRight();
}



}



void Drive(){
Serial.println("DRIVING");
LeftMotor->setSpeed(50);
RightMotor->setSpeed(50);
LeftMotor->run(FORWARD);
RightMotor->run(FORWARD);
delay(100);
}

void ShiftLeft(){
Serial.println("SHIFTING LEFT");
LeftMotor->setSpeed(70);
LeftMotor->run(BACKWARD);
delay(200);
Brake();
}

void ShiftRight(){
Serial.println("SHIFTING RIGHT");
RightMotor->setSpeed(70);
RightMotor->run(BACKWARD);
delay(200);
Brake();
}

void Brake(){
Serial.println("BRAKING");
LeftMotor->run(RELEASE);
RightMotor->run(RELEASE);

}

