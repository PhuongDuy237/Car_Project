# include "car_control.h"

const int hornFreq = 2600;

CarController::CarController() :
inRight1(4), inRight2(5), inLeft1(6), inLeft2(7), hornVcc(9), hornGnd(8)
{}

CarController::CarController(const int inRight1, const int inRight2, const int inLeft1, const int inLeft2, const int hornVcc, const int hornGnd) :
inRight1(inRight1), inRight2(inRight2), inLeft1(inLeft1), inLeft2(inLeft2), hornVcc(hornVcc), hornGnd(hornGnd) 
{}

void CarController::stop() {
  digitalWrite(inLeft1, LOW);
  digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, LOW);
  digitalWrite(inRight2, LOW);
}

void CarController::moveForward() {
  digitalWrite(inLeft1, HIGH);
  digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH);
  digitalWrite(inRight2, LOW);
}

void CarController::moveBackward() {
  digitalWrite(inLeft1, LOW);
  digitalWrite(inLeft2, HIGH);
  digitalWrite(inRight1, LOW);
  digitalWrite(inRight2, HIGH);
}

void CarController::turnLeft() {
  digitalWrite(inLeft1, HIGH);
  digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, LOW);
  digitalWrite(inRight2, HIGH);
}

void CarController::turnRight() {
  digitalWrite(inLeft1, LOW);
  digitalWrite(inLeft2, HIGH);
  digitalWrite(inRight1, HIGH);
  digitalWrite(inRight2, LOW);
}

void CarController::setupHorn() {
  pinMode(hornVcc, OUTPUT);
  pinMode(hornGnd, OUTPUT);

  digitalWrite(hornGnd, LOW);
  noTone(hornVcc);
}

void CarController::hornOn() {
  tone(hornVcc, hornFreq);
}

void CarController::hornOff() {
  noTone(hornVcc);
}

void CarController::setupMotors() {
  pinMode(inLeft1, OUTPUT);
  pinMode(inLeft2, OUTPUT);
  pinMode(inRight1, OUTPUT);
  pinMode(inRight2, OUTPUT);

  stop();
}