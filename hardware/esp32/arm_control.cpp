#include "arm_control.h"

// base - shoulder - elbow - gripper
const int minAngles[] = {0, 60, 40, 58};
const int maxAngles[] = {180, 180, 180, 109};

ArmController::ArmController(){
    this->basePin = 25;
    this->shoulderPin = 18;
    this->elbowPin = 19;
    this->gripperPin = 21;

    for (int i = 0; i < 4; ++i){
        this->currentAngles[i] = 90;
    }

    this->baseServo = Servo();
    this->shoulderServo = Servo();
    this->elbowServo = Servo();
    this->gripperServo = Servo();
}

ArmController::ArmController(int basePin, int shoulderPin, int elbowPin, int gripperPin){
    this->basePin = basePin;
    this->shoulderPin = shoulderPin;
    this->elbowPin = elbowPin;
    this->gripperPin = gripperPin;

    for (int i = 0; i < 4; ++i){
        this->currentAngles[i] = 90;
    }

    this->baseServo = Servo();
    this->shoulderServo = Servo();
    this->elbowServo = Servo();
    this->gripperServo = Servo();
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

void ArmController::stopAll(){
    int stopBase = this->baseServo.read();
    int stopShoulder = this->shoulderServo.read();
    int stopElbow = this->elbowServo.read();
    int stopGripper = this->gripperServo.read();

    this->baseServo.write(stopBase);
    this->shoulderServo.write(stopShoulder);
    this->elbowServo.write(stopElbow);
    this->gripperServo.write(stopGripper);

    this->currentAngles[0] = stopBase;
    this->currentAngles[1] = stopShoulder;
    this->currentAngles[2] = stopElbow;
    this->currentAngles[3] = stopGripper;
}

void ArmController::moveSlow(Servo& servo, int currentAngle, int targetAngle, int speedDelay = 15){
    if (currentAngle < targetAngle){
        for (int pos = currentAngle; pos <= targetAngle; ++pos){
            servo.write(pos);
            delay(speedDelay);
        }
    }
    else{
        for (int pos = currentAngle; pos >= targetAngle; --pos){
            servo.write(pos);
            delay(speedDelay);
        }
    }

    currentAngle = targetAngle;
}

void ArmController::moveJoint(int joint, int targetAngle){
    if (targetAngle < minAngles[joint])     targetAngle = minAngles[joint];
    if (targetAngle > maxAngles[joint])     targetAngle = maxAngles[joint];

    switch(joint){
        case '0': moveSlow(baseServo, currentAngles[0], targetAngle); break;
        case '1': moveSlow(shoulderServo, currentAngles[0], targetAngle); break;
        case '2': moveSlow(elbowServo, currentAngles[0], targetAngle); break;
        case '3': moveSlow(gripperServo, currentAngles[0], targetAngle); break;
    }
}