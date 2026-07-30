#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

#include <Arduino.h>

extern const int basePin;
extern const int shoulderPin;
extern const int elbowPin;
extern const int gripperPin;

void setupArm();
int getCurrentAngle(int joint);
void moveJoint(int joint, int targetAngle);

#endif