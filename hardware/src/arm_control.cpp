#include "arm_control.h"
#include <Servo.h>

const int basePin = 10;
const int shoulderPin = 11;
const int elbowPin = 12;
const int gripperPin = 13;

Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// base - shoulder - elbow - gripper
int currentAngles[] = {90, 90, 90, 90};
const int minAngles[] = {0, 60, 40, 58};
const int maxAngles[] = {180, 180, 180, 109};

void setupArm() {
    baseServo.attach(basePin);
    shoulderServo.attach(shoulderPin);
    elbowServo.attach(elbowPin);
    gripperServo.attach(gripperPin);

    moveJoint(0, currentAngles[0]);
    moveJoint(1, currentAngles[1]);
    moveJoint(2, currentAngles[2]);
    moveJoint(3, currentAngles[3]);
}

int getCurrentAngle(int joint) {
    if (joint >= 0 && joint <= 3){
        return currentAngles[joint];
    }
    return -1;
}

void moveJoint(int joint, int targetAngle) {
    if (targetAngle < minAngles[joint])     targetAngle = minAngles[joint];
    if (targetAngle > maxAngles[joint])     targetAngle = maxAngles[joint];

    switch(joint) {
        case 0: baseServo.write(targetAngle);            break;
        case 1: shoulderServo.write(targetAngle);        break;
        case 2: elbowServo.write(targetAngle);           break;
        case 3: gripperServo.write(targetAngle);         break;
    }

    currentAngles[joint] = targetAngle;
}