#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(4);


bool turn1 = true;
bool turn2 = false;
bool turn3 = false;


long duration;
float distance;

int wallGap = LOW;

const int trigPin_10 = 10;
const int echoPin_9 = 9;
const int leftWallPin_12 = 12;

const int lowSpeed = 35;//33 --25
const int medSpeed = 40;//40
const int fastSpeed = 47;//55

void setup() {
pinMode(trigPin_10, OUTPUT); 
pinMode(echoPin_9, INPUT); 
pinMode(leftWallPin_12,INPUT);
Serial.begin(9600);
AFMS.begin();



}

//Hit wall, Turn Right
//Hit Wall, Turn right
//Hit wall turn left

void loop() {
    long currentDistance = ping();
    wallGap = digitalRead(leftWallPin_12);
    /*
    if(wallGap == HIGH){
      //This means the robot is closing in on the leftwall and should turn Right
       ShiftRight(medSpeed,true);
       Brake();
       delay(100);
    }
    else if (wallGap == LOW){
       Drive(medSpeed);
       Brake();
       delay(100);
    }
    */
     if (currentDistance < 10){
      //turn right
      if(turn1){
        turn1 = false;
        turn2 = true;
        ShiftRight(medSpeed,false);
        Serial.print("Turn 1");
        
      }
      else if(turn2){
        //turn right
        turn2 = false;
        turn3 = true;
        ShiftRight(medSpeed,false);
        Serial.print("Turn 2");
      }
      else if(turn3){
        //turn left
        turn3 = false;
        turn2 = false;
        ShiftLeft(medSpeed,false);
        Serial.print("Turn 3");
      }
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
    


long ping(){//returns distance in Cm
  
    digitalWrite(trigPin_10, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin_10, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_10, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin_9, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");  
    delay(1000);
    return distance;
    
}




