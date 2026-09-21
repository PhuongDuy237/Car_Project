#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H

#include <Arduino.h>

class CarController {
    private:
    const int inRight1;
    const int inRight2;
    const int inLeft1;
    const int inLeft2;
    const int hornVcc;
    const int hornGnd;

    public:
    CarController();
    CarController(const int inRight1, const int inRight2, const int inLeft1, const int inLeft2, const int hornVcc, const int hornGnd);
    void setupMotors();
    void stop();
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    void setupHorn();
    void hornOn();
    void hornOff();
};

#endif