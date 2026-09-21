#include "arm_control.h"

// base - shoulder - elbow - gripper
const int minAngles[] = {0, 60, 40, 58};
const int maxAngles[] = {180, 180, 180, 109};
const int speed = 40;

ArmController::ArmController(){
    this->basePin = 16;
    this->shoulderPin = 17;
    this->elbowPin = 5;
    this->gripperPin = 18;

    for (int i = 0; i < 4; ++i){
        this->currentAngles[i] = 90;
    }

    this->baseServo = ServoEasing();
    this->shoulderServo = ServoEasing();
    this->elbowServo = ServoEasing();
    this->gripperServo = ServoEasing();
}

ArmController::ArmController(int basePin, int shoulderPin, int elbowPin, int gripperPin){
    this->basePin = basePin;
    this->shoulderPin = shoulderPin;
    this->elbowPin = elbowPin;
    this->gripperPin = gripperPin;

    for (int i = 0; i < 4; ++i){
        this->currentAngles[i] = 90;
    }

    this->baseServo = ServoEasing();
    this->shoulderServo = ServoEasing();
    this->elbowServo = ServoEasing();
    this->gripperServo = ServoEasing();
}

int ArmController::getCurrentAngle(int joint){
    if (joint >= 0 && joint <= 3)
        return currentAngles[joint];
    return -1;
}

void ArmController::setupArm(){
    this->baseServo.attach(basePin);
    this->shoulderServo.attach(shoulderPin);
    this->elbowServo.attach(elbowPin);
    this->gripperServo.attach(gripperPin);

    moveJoint(0, currentAngles[0]);
    moveJoint(1, currentAngles[1]);
    moveJoint(2, currentAngles[2]);
    moveJoint(3, currentAngles[3]);
}

void ArmController::moveJointToLimit(int joint, int direction){
    // direction > 0 chay toi kich kim Max, < 0 thi lui ve kich kim Min.
    // Dung startEaseTo (chay nen qua interrupt) thay vi easeTo (block ca loop())
    // de con nhan lenh moi/lenh dung trong luc servo dang di chuyen.
    int targetAngle = (direction > 0) ? maxAngles[joint] : minAngles[joint];

    switch(joint){
        case 0: baseServo.startEaseTo(targetAngle, speed); break;
        case 1: shoulderServo.startEaseTo(targetAngle, speed); break;
        case 2: elbowServo.startEaseTo(targetAngle, speed); break;
        case 3: gripperServo.startEaseTo(targetAngle, speed); break;
    }
}

void ArmController::stopAll(){
    baseServo.stop();
    shoulderServo.stop();
    elbowServo.stop();
    gripperServo.stop();

    currentAngles[0] = baseServo.read();
    currentAngles[1] = shoulderServo.read();
    currentAngles[2] = elbowServo.read();
    currentAngles[3] = gripperServo.read();
}

// void ArmController::moveSlow(ServoEasing& servo, int currentAngle, int targetAngle, int speedDelay = 15){
//     if (currentAngle < targetAngle){
//         for (int pos = currentAngle; pos <= targetAngle; ++pos){
//             servo.easeTo(pos);
//             delay(speedDelay);
//         }
//     }
//     else{
//         for (int pos = currentAngle; pos >= targetAngle; --pos){
//             servo.easeTo(pos);
//             delay(speedDelay);
//         }
//     }

//     currentAngle = targetAngle;
// }

void ArmController::moveJoint(int joint, int targetAngle){
    if (targetAngle < minAngles[joint])     targetAngle = minAngles[joint];
    if (targetAngle > maxAngles[joint])     targetAngle = maxAngles[joint];

    switch(joint){
        case 0: baseServo.easeTo(targetAngle, speed); break;
        case 1: shoulderServo.easeTo(targetAngle, speed); break;
        case 2: elbowServo.easeTo(targetAngle, speed); break;
        case 3: gripperServo.easeTo(targetAngle, speed); break;
    }

    currentAngles[joint] = targetAngle;
}