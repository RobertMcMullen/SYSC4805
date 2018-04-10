//NECESSARY LIBRARIES FOR MOTOR SHIELD
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(4);

int IR_Pin = LOW;
long duration;
float distance;

//IR SENSOR FOR LEFT WALL DETECTION
int wallGap = LOW;

const int trigPin_10 = 10;
const int echoPin_9 = 9;
const int leftWallPin_12 = 12;

//TURNING SPEEDS
const int lowSpeed = 34;
const int medSpeed = 40;
const int fastSpeed = 47;

void setup() {
pinMode(trigPin_10, OUTPUT); 
pinMode(echoPin_9, INPUT); 
pinMode(leftWallPin_12,INPUT);
Serial.begin(9600);
AFMS.begin();
int turns = 1;
}


void loop() {

    long currentDistance = ping();    //updating Ultra sonic sensor range detection 
     IR_Pin = digitalRead(leftWallPin_12); //reading IR sensor to detect left walls
     //ultrasonic range is 14cm, varaible as motors have latency and can overshoot
      if (currentDistance < 14){
          Brake();
          if(turns == 1){//first maze turn
              Serial.println("SHIFTING LEFT");
              ShiftRight(lowSpeed);
              ShiftRight(lowSpeed);
          }
          else if(turns == 2){//second maze turn
              Serial.println("SHIFTING RIGHT");
              ShiftRight(lowSpeed);
              ShiftRight(lowSpeed);
          }
          else if(turns == 3){//third maze turn
              Serial.println("SHIFTING RIGHT");
              ShiftLeft(lowSpeed);
              ShiftLeft(lowSpeed);
          }     
      turns = turns + 1; // increment the turn count, no more turns will be taken, will simply drive staight
    }
    Drive(lowSpeed);

}//end loop





void Drive(int speed){
     //veering funciton used in the drive function as it should veer instead of drive if need be
     //ultrasonic detection was getting lagged with too many drive (veer, turn, drive, etc) functions in the loop 
     if (IR_Pin == LOW){
      //LEFT WALL DETECTION
      Serial.println("VEERING RIGHT");
      veerRight(lowSpeed);
      IR_Pin = HIGH;
     }
//code below is required to drive the car straight operating both motors
    LeftMotor->setSpeed(speed);
    RightMotor->setSpeed(speed);
    LeftMotor->run(FORWARD);
    RightMotor->run(FORWARD);
    delay(80);
}

//function to slightly turn the vehicle (only operates a single motor)
//used as counteraction to the single IR sensor on left side of vehicle
void veerRight(int speed){
    //Speed is hardcoded here as detection distance was adjusted on the sensor itself, had to fine tune
    RightMotor->setSpeed(35);
    RightMotor->run(BACKWARD);
    delay(275);
}

//function to turn the vehicle left (operates both motors)
void ShiftLeft(int speed){
    RightMotor->setSpeed(speed);
    LeftMotor->setSpeed(speed*2);
    RightMotor->run(FORWARD);
    LeftMotor->run(BACKWARD);
    delay(430);
    Brake();
}

//function to turn the vehicle right (operates both motors)
void ShiftRight(int speed){
    RightMotor->setSpeed(speed*2);
    LeftMotor->setSpeed(speed);
    RightMotor->run(BACKWARD);
    LeftMotor->run(FORWARD);
    delay(410);
    Brake();
}

//function to brake the vehicle (release and stop driving the motors)
void Brake(){
    //Serial.println("BRAKING");
    LeftMotor->run(RELEASE);
    RightMotor->run(RELEASE);
    }
    

//ultrasonic detection function
//returns distance in Cm
long ping(){

  
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


