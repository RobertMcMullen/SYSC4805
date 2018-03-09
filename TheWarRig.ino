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

bool turn1 = false;
bool turn2 = false;
bool turn3 = false;
bool turn4 = false;
bool turn5 = false;
bool turn6 = false;

bool finished = false;



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

const int lowSpeed = 35;//33 --25
const int medSpeed = 40;//40
const int fastSpeed = 47;//55

void setup() {
Serial.begin(9600);
AFMS.begin();
pinMode(pin12D, INPUT);
pinMode(pin11B, INPUT);
pinMode(pin4A, INPUT);
pinMode(pin3C, INPUT);
pinMode(pin2E, INPUT);


}

/*
 * Left,Right,Right,Left,Right,Right Follow
 * 
 */
void loop() {
FarLeft = digitalRead(pin12D);
Left = digitalRead(pin11B);
Center = digitalRead(pin4A);
Right = digitalRead(pin3C);
FarRight = digitalRead(pin2E);



//END
if(((FarRight == LOW) && (Right == LOW) && (Center == LOW) && (Left == LOW)) || ((FarLeft == LOW) && (Right == LOW) && (Center == LOW) && (Left == LOW))){
  Drive(lowSpeed);
  if(((FarRight == LOW) && (Right == LOW) && (Center == LOW) && (Left == LOW)) || ((FarLeft == LOW) && (Right == LOW) && (Center == LOW) && (Left == LOW))){
    Brake();  
    Serial.println("FINISHED");  
    finished=true;
  }
}


else if(turn1 == false && FarLeft == LOW){
  turn1  = true;
  turnLeft(medSpeed);
  Brake();
  delay(100);
}
else if(turn1 == true && turn2 == false && FarRight == LOW){
  turn2 = true;
  turnRight(medSpeed);
  Brake();
  delay(100);
}

else if(turn1 == true && turn2 == true && turn3 == false && FarLeft == LOW){
  turn3 = true;
  turnLeft(medSpeed);
  Brake();
  delay(100);
}
else if(turn1 == true && turn2 == true && turn3 == true && turn4 == false  && FarRight == LOW){
  turn4 = true;
  turnRight(medSpeed);
  Brake();
  delay(100);
}
else if(turn1 == true && turn2 == true && turn3 == true && turn4 == true && turn5 == false && FarRight == LOW){
  turn5 = true;
  turnRight(medSpeed);
  Brake();
  delay(100);
}

else if((FarLeft == HIGH) && (Left == HIGH) && (Right == HIGH) && (FarRight==HIGH) && (Center == HIGH) && finished == false){
  
  Drive(medSpeed);    
  Brake();
  delay(100);  
}

//Straight Ahead
else if((FarLeft == HIGH) && (Left == HIGH) && (Right == HIGH) && (FarRight==HIGH) && (Center == LOW)){
  Serial.println("1"); 
  if(turn6){
    Drive(lowSpeed);
  }
  else {
    Drive(medSpeed);
  }
  
}
//Easy Left or Small correction Left
else if(Left == LOW && FarLeft == HIGH){
  Serial.println("6"); 
  Brake();
  ShiftLeft(medSpeed,true);
}

//Easy Right or Small correction Right
else if(Right == LOW && FarRight == HIGH){
  Serial.println("2"); 
  Brake();
  ShiftRight(medSpeed,true);
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
}//end loop




void maintain(){

  
FarLeft = digitalRead(pin12D);
Left = digitalRead(pin11B);
Center = digitalRead(pin4A);
Right = digitalRead(pin3C);
FarRight = digitalRead(pin2E);


//Straight Ahead
if((FarLeft == HIGH) && (Left == HIGH) && (Right == HIGH) && (FarRight==HIGH) && (Center == LOW)){
  Serial.println("1"); 
  Drive(lowSpeed);
}
//Easy Right or Small correction Right
else if(Right == LOW && FarRight == HIGH){
  Serial.println("2"); 
  Brake();
  ShiftRight(fastSpeed,true);
}
//Easy Left or Small correction Left
else if(Left == LOW && FarLeft == HIGH){
  Serial.println("6"); 
  Brake();
  ShiftLeft(fastSpeed,true);
}

}//end loop



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

void turnLeft(int speed){
//Serial.println("SHIFTING LEFT");
while(true){
  RightMotor->setSpeed(speed);
  RightMotor->run(FORWARD);
  delay(100);
  if(digitalRead(pin4A) == LOW){
    break;
  }
}
delay(200);
Brake();
}


void turnRight(int speed){
//Serial.println("SHIFTING LEFT");
while(true){
  LeftMotor->setSpeed(speed);
  LeftMotor->run(FORWARD);
  delay(100);
  if(digitalRead(pin4A) == LOW){
    break;
  }
}
delay(200);
Brake();
}





