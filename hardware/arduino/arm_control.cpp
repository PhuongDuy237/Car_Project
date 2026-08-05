#include "arm_control.h"

// base - shoulder - elbow - gripper
const int minAngles[] = {0, 60, 40, 58};
const int maxAngles[] = {180, 180, 180, 109};

ArmController::ArmController(int basePin, int shoulderPin, int elbowPin, int gripperPin) {
    this->basePin = basePin;
    this->shoulderPin = shoulderPin;
    this->elbowPin = elbowPin;
    this->gripperPin = gripperPin;

    for (int i = 0; i < 4; i++){
        this->currentAngles[i] = 90;
    }

    this->gripperServo = VarSpeedServo();
    this->baseServo = VarSpeedServo();
    this->shoulderServo = VarSpeedServo();
    this->elbowServo = VarSpeedServo();
}

ArmController::ArmController() {
    this->basePin = 10;
    this->shoulderPin = 11;
    this->elbowPin = 12;
    this->gripperPin = 13;

    for (int i = 0; i < 4; i++){
        this->currentAngles[i] = 90;
    }

    this->gripperServo = VarSpeedServo();
    this->baseServo = VarSpeedServo();
    this->shoulderServo = VarSpeedServo();
    this->elbowServo = VarSpeedServo();
}

void ArmController::setupArm() {
    this->baseServo.attach(basePin);
    this->shoulderServo.attach(shoulderPin);
    this->elbowServo.attach(elbowPin);
    this->gripperServo.attach(gripperPin);

    moveJoint(0, currentAngles[0]);
    moveJoint(1, currentAngles[1]);
    moveJoint(2, currentAngles[2]);
    moveJoint(3, currentAngles[3]);
}

int ArmController::getCurrentAngle(int joint) {
    if (joint >= 0 && joint <= 3){
        return currentAngles[joint];
    }
    return -1;
}

void ArmController::moveJoint(int joint, int targetAngle) {
    if (targetAngle < minAngles[joint])     targetAngle = minAngles[joint];
    if (targetAngle > maxAngles[joint])     targetAngle = maxAngles[joint];

    switch(joint) {
        case 0: baseServo.write(targetAngle, 20, false);            break;
        case 1: shoulderServo.write(targetAngle, 20, false);        break;
        case 2: elbowServo.write(targetAngle, 20, false);           break;
        case 3: gripperServo.write(targetAngle, 20, false);         break;
    }

    currentAngles[joint] = targetAngle;
}

void ArmController::controlGripper() {}

void ArmController::moveJointToLimit(int joint, int direction) {
    // direction > 0 thì chạy tới kịch kim Max, < 0 thì lùi về kịch kim Min
    int targetAngle = (direction > 0) ? maxAngles[joint] : minAngles[joint];
    
    switch(joint) {
        case 0: baseServo.write(targetAngle, 20, false); break;
        case 1: shoulderServo.write(targetAngle, 20, false); break;
        case 2: elbowServo.write(targetAngle, 20, false); break;
        case 3: gripperServo.write(targetAngle, 20, false); break;
    }
}

void ArmController::stopAll() {
    // 1. Kích hoạt phanh gấp cho tất cả các khớp
    baseServo.stop();
    shoulderServo.stop();
    elbowServo.stop();
    gripperServo.stop();

    // 2. Cập nhật lại "trí nhớ" ngay tại vị trí vừa phanh
    currentAngles[0] = baseServo.read();
    currentAngles[1] = shoulderServo.read();
    currentAngles[2] = elbowServo.read();
    currentAngles[3] = gripperServo.read();
}
// void controlGripperByAI() {
//     if (Serial.available() > 0) {
//         char command = Serial.read();
        
//         if (command == 'G') {
//             moveJoint(3, 58); 
//         } 
//         else if (command == 'M') {
//             moveJoint(3, 109);
//         }
//     }
// }