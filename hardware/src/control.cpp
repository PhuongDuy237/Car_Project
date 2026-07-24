# include "control.h"

// Left pair
const int inRight1 = 4;
const int inRight2 = 5;

// Right pair
const int inLeft1 = 6;
const int inLeft2 = 7;

// Horn
const int hornGnd = 8;
const int hornVcc = 9;
const int hornFreq = 2600;

void stop() {
  digitalWrite(inLeft1, LOW);
  digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, LOW);
  digitalWrite(inRight2, LOW);
}

void moveForward() {
  digitalWrite(inLeft1, HIGH);
  digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, HIGH);
  digitalWrite(inRight2, LOW);
}

void moveBackward() {
  digitalWrite(inLeft1, LOW);
  digitalWrite(inLeft2, HIGH);
  digitalWrite(inRight1, LOW);
  digitalWrite(inRight2, HIGH);
}

void turnLeft() {
  digitalWrite(inLeft1, HIGH);
  digitalWrite(inLeft2, LOW);
  digitalWrite(inRight1, LOW);
  digitalWrite(inRight2, HIGH);
}

void turnRight() {
  digitalWrite(inLeft1, LOW);
  digitalWrite(inLeft2, HIGH);
  digitalWrite(inRight1, HIGH);
  digitalWrite(inRight2, LOW);
}

void setupHorn() {
  pinMode(hornVcc, OUTPUT);
  pinMode(hornGnd, OUTPUT);

  digitalWrite(hornGnd, LOW);
  noTone(hornVcc);
}

void hornOn() {
  tone(hornVcc, hornFreq);
}

void hornOff() {
  noTone(hornVcc);
}

void setupMotors() {
  pinMode(inLeft1, OUTPUT);
  pinMode(inLeft2, OUTPUT);
  pinMode(inRight1, OUTPUT);
  pinMode(inRight2, OUTPUT);

  stop();
}