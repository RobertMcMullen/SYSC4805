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

bool started = false;

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

const int lowSpeed = 33;//33 --25
const int medSpeed = 30;//40
const int fastSpeed = 40;//55

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
FarLeft = digitalRead(pin12D);
Left = digitalRead(pin11B);
Center = digitalRead(pin4A);
Right = digitalRead(pin3C);
FarRight = digitalRead(pin2E);

//Denotes the start of the maze, from this point on, if all the lights go HIGH then it must turn around
if((started == false) && ((FarRight == LOW) || (Right == LOW) || (Center == LOW) || (Left == LOW) || (FarLeft == LOW))){
  started = true;
}

//END
else if((FarRight == LOW) && (Right == LOW) && (Center == LOW) && (Left == LOW) && (FarLeft == LOW)){
  Brake();  
  Serial.println("FINISHED");  
}

//Straight Ahead
else if((FarLeft == HIGH) && (Left == HIGH) && (Right == HIGH) && (FarRight==HIGH) && (Center == LOW)){
  Serial.println("1"); 
  Drive(lowSpeed);
}
//Turn around 
else if((FarLeft == HIGH) && (Left == HIGH) && (Right == HIGH) && (FarRight==HIGH) && (Center == HIGH) && (started == true)){
  Serial.println("0"); 
  turnAround(medSpeed);
}
//Hard Right or 90deg. right turn
else if(Right == LOW && FarRight == LOW){
  Serial.println("3"); 
  Brake();
  ShiftRight(medSpeed,true);
  delay(100);
}
//Hard Right or 90deg. right turn
else if(FarRight == LOW && Center == LOW){//Right == LOW && 
  Serial.println("4"); 
  Brake();
  ShiftRight(medSpeed,false);
}
//Easy Right or Small correction Right
else if(Right == LOW && FarRight == HIGH){
  Serial.println("2"); 
  Brake();
  ShiftRight(fastSpeed,true);
}

//Needs to over correct left, just the outside caught the tape
else if((Center == HIGH)&& (Left == HIGH) && (FarLeft==LOW)){
  Serial.println("5"); 
  Brake();
  ShiftLeft(medSpeed,false);
}
//Hard Left or 90deg. left
else if(Left == LOW && FarLeft == LOW){
  Serial.println("7"); 
  Brake();
  ShiftLeft(medSpeed,true);
}
//Needs to over correct right, just the outside caught the tape
else if((Center == HIGH)&& (Right == HIGH) && (FarRight==LOW)){
  Serial.println("8"); 
  Brake();
  ShiftRight(medSpeed,false);
}
//Easy Left or Small correction Left
else if(Left == LOW && FarLeft == HIGH){
  Serial.println("6"); 
  Brake();
  ShiftLeft(fastSpeed,true);
}

//Easy Left or Small correction Left
else if(Left == LOW && FarLeft == LOW){
  Serial.println("9"); 
  Brake();
  ShiftLeft(fastSpeed,true);
}

//Already past the point of making a decision Keep moving fwd
else if(FarLeft == LOW && Left== HIGH && Center == LOW && Right == HIGH){
  Serial.println("10"); 
  Brake();
  Drive(lowSpeed);
}

}



void Drive(int speed){
//Serial.println("DRIVING");
LeftMotor->setSpeed(speed);
RightMotor->setSpeed(speed);
LeftMotor->run(FORWARD);
RightMotor->run(FORWARD);
delay(100);
}

void ShiftLeft(int speed,bool dir){
//Serial.println("SHIFTING LEFT");
RightMotor->setSpeed(speed);
if(dir){
  RightMotor->run(FORWARD);
}
else{
  LeftMotor->run(BACKWARD);
  LeftMotor->setSpeed(speed*2);
}
delay(200);
Brake();
}

void ShiftRight(int speed,bool dir){
//Serial.println("SHIFTING RIGHT");
LeftMotor->setSpeed(speed);
if(dir){
  LeftMotor->run(FORWARD);
}
else{
  RightMotor->run(BACKWARD);  
  RightMotor->setSpeed(speed*2);  
}
delay(200);
Brake();
}

void Brake(){
//Serial.println("BRAKING");
LeftMotor->run(RELEASE);
RightMotor->run(RELEASE);
}

//Turns around CCW
void turnAround(int speed){
  for (int i =0; i <200;i++){
  if(digitalRead(pin4A) == LOW){
    reverse(speed);
    break;
  }
  RightMotor->run(FORWARD); 
  LeftMotor->run(BACKWARD);
  RightMotor->setSpeed(fastSpeed);  
  LeftMotor->setSpeed(fastSpeed);  
  delay(10);
  }
  Brake();
}
void reverse(int speed){
  while(digitalRead(pin4A) == LOW){
  RightMotor->run(BACKWARD); 
  LeftMotor->run(BACKWARD);
  RightMotor->setSpeed(fastSpeed);  
  LeftMotor->setSpeed(fastSpeed);  
  delay(10);
  }
  /*
  while(digitalRead(pin4A) == HIGH){
  RightMotor->run(FORWARD); 
  LeftMotor->run(FORWARD);
  RightMotor->setSpeed(speed*2);  
  LeftMotor->setSpeed(speed*2);  
  delay(10);
  }
  */
  
}
/*
void fwdCheck(int inputPin){
  drive(medSpeed);
  
}*/




