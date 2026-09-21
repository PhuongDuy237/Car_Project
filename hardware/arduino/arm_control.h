#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

#include <Arduino.h>
#include <VarSpeedServo.h>

class ArmController {
private:
    int basePin;
    int shoulderPin;
    int elbowPin;
    int gripperPin;
    int currentAngles[4];

    VarSpeedServo baseServo;
    VarSpeedServo shoulderServo;
    VarSpeedServo elbowServo;
    VarSpeedServo gripperServo;

public:
    ArmController();
    ArmController(int basePin, int shoulderPin, int elbowPin, int gripperPin);

    void setupArm();
    int getCurrentAngle(int joint);
    void moveJoint(int joint, int targetAngle);
    void controlGripper();

    void moveJointToLimit(int joint, int direction);
    void stopAll();
};

#endif