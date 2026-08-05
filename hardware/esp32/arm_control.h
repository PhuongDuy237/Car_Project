#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h>

class ArmController{
private:
    int basePin;
    int shoulderPin;
    int elbowPin;
    int gripperPin;
    int currentAngles[4];

    Servo baseServo;
    Servo shoulderServo;
    Servo elbowServo;
    Servo gripperServo;

    void moveSlow(Servo& servo, int currentAngle, int targetAngle, int speedDelay = 15);
public:
    ArmController();
    ArmController(int basePin, int shoulderPin, int elbowPin, int gripperPin);
    void setupArm();
    int getCurrentAngle(int joint);
    void moveJoint(int joint, int targetAngle);
    void stopAll();
};

#endif