# include "car_control.h"

CarController::CarController() :
    rightMotors{13, 14, 22},
    leftMotors{26, 27, 23},
    sonar{16, 35},
    hornPin(32),
    speed{0, 70, 135, 185, 255}
{}

CarController::CarController(int inRight1, int inRight2, int enA, int inLeft1, int inLeft2, int enB, int hornPin, int trigPin, int echoPin) : 
    rightMotors{inRight1, inRight2, enA},
    leftMotors{inLeft1, inLeft2, enB},
    sonar{trigPin, echoPin},
    hornPin(hornPin),
    speed{0, 70, 135, 185, 255} 
{}

void CarController::setupCar() {
  // Set up motors
  pinMode(rightMotors.in1, OUTPUT);
  pinMode(rightMotors.in2, OUTPUT);
  pinMode(rightMotors.en, OUTPUT);
  pinMode(leftMotors.in1, OUTPUT);
  pinMode(leftMotors.in2, OUTPUT);
  pinMode(leftMotors.en, OUTPUT);

  stop();

  //Set up horn
  pinMode(hornPin, OUTPUT);
  noTone(hornPin);

  //Set up HC-SR04
  pinMode(sonar.trig, OUTPUT);
  pinMode(sonar.echo, INPUT);
}

void CarController::stop() {
  digitalWrite(leftMotors.in1, LOW);
  digitalWrite(leftMotors.in2, LOW);
  digitalWrite(rightMotors.in1, LOW);
  digitalWrite(rightMotors.in2, LOW);
}

void CarController::moveForward(int speedLv) {
  digitalWrite(leftMotors.in1, HIGH);
  digitalWrite(leftMotors.in2, LOW);
  analogWrite(leftMotors.en, speed[speedLv]);
  
  digitalWrite(rightMotors.in1, HIGH);
  digitalWrite(rightMotors.in2, LOW);
  analogWrite(rightMotors.en, speed[speedLv]);
}

void CarController::moveBackward(int speedLv) {
  digitalWrite(leftMotors.in1, LOW);
  digitalWrite(leftMotors.in2, HIGH);
  analogWrite(leftMotors.en, speed[speedLv]);

  digitalWrite(rightMotors.in1, LOW);
  digitalWrite(rightMotors.in2, HIGH);
  analogWrite(rightMotors.en, speed[speedLv]);
}

void CarController::turnLeft(int speedLv) {
  digitalWrite(leftMotors.in1, HIGH);
  digitalWrite(leftMotors.in2, LOW);
  analogWrite(leftMotors.en, speed[speedLv]);

  digitalWrite(rightMotors.in1, LOW);
  digitalWrite(rightMotors.in2, HIGH);
  analogWrite(rightMotors.en, speed[speedLv]);
}

void CarController::turnRight(int speedLv) {
  digitalWrite(leftMotors.in1, LOW);
  digitalWrite(leftMotors.in2, HIGH);
  analogWrite(leftMotors.en, speed[speedLv]);

  digitalWrite(rightMotors.in1, HIGH);
  digitalWrite(rightMotors.in2, LOW);
  analogWrite(rightMotors.en, speed[speedLv]);
}

void CarController::hornOn(int hornFreq) {
  tone(hornPin, hornFreq);
}

void CarController::hornOff() {
  noTone(hornPin);
}

int CarController::getDistance(){
  digitalWrite(sonar.trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar.trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar.trig, LOW);

  long duration = pulseIn(sonar.echo, HIGH, 30000);

  if (duration == 0)    return 999;

  return (0.034 * duration) / 2;
}