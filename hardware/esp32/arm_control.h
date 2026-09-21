#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h>
// File nay khong phai file main (khong co setup()/loop()), nen chi include
// ban khai bao nhe <ServoEasing.h>, khong phai <ServoEasing.hpp> (ban cai dat
// day du) - neu khong se bi loi "multiple definition" luc link.
#define SUPPRESS_HPP_WARNING
#include <ServoEasing.h>

class ArmController{
private:
    int basePin;
    int shoulderPin;
    int elbowPin;
    int gripperPin;
    int currentAngles[4];

    ServoEasing baseServo;
    ServoEasing shoulderServo;
    ServoEasing elbowServo;
    ServoEasing gripperServo;

    // void moveSlow(ServoEasing& servo, int currentAngle, int targetAngle, int speedDelay = 15);
public:
    ArmController();
    ArmController(int basePin, int shoulderPin, int elbowPin, int gripperPin);
    void setupArm();
    int getCurrentAngle(int joint);
    void moveJoint(int joint, int targetAngle);
    void moveJointToLimit(int joint, int direction);
    void stopAll();
};

#endif