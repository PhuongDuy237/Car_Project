#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H

#include <Arduino.h>

extern const int inLeft1;
extern const int inLeft2;
extern const int inRight1;
extern const int inRight2;
extern const int hornVcc;
extern const int hornGnd;

void setupMotors();
void stop();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void setupHorn();
void hornOn();
void hornOff();

#endif