#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H

#include <Arduino.h>

struct MotorPins {
    int in1;
    int in2;
    int en;
};

struct SonarPins {
    int trig;
    int echo;
};

class CarController {
    private:
    MotorPins rightMotors;
    MotorPins leftMotors;
    SonarPins sonar;
    int hornPin;
    int speed[5];

    public:
    CarController();
    CarController(
        int inRight1, int inRight2, int enA, int inLeft1, int inLeft2, int enB,
        int hornPin, int trigPin, int echoPin);
    void setupCar();
    void moveForward(int speedLv);
    void moveBackward(int speedLv);
    void turnLeft(int speedLv);
    void turnRight(int speedLv);
    void stop();
    void hornOn(int hornFreq);
    void hornOff();
    int getDistance();
    int getSpeed();
};

#endif