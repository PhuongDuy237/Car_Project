#include <Arduino.h>
#include <SoftwareSerial.h>
#include "car_control.h"
#include "arm_control.h"

SoftwareSerial myBluetooth(2, 3); // Tx, Rx
char command;
CarController* car = new CarController();


void setup() {
Serial.begin(9600);
myBluetooth.begin(9600);

car->setupMotors();
car->setupHorn();
}

void loop() {
  if (myBluetooth.available() > 0){
    command = myBluetooth.read();
    Serial.println("Command recieved: " + String(command));

    switch(command){
        case 'F': car->moveForward(); break;
        case 'B': car->moveBackward(); break;
        case 'L': car->turnLeft(); break;
        case 'R': car->turnRight();  break;
        case 'S': car->stop(); break;
        case 'V': car->hornOn(); break;
        case 'v': car->hornOff(); break;
        }
    }
}