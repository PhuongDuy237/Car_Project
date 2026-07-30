#include <Arduino.h>
#include <SoftwareSerial.h>
#include "car_control.h"
#include "arm_control.h"

SoftwareSerial myBluetooth(2, 3); // Tx, Rx
char command;

void setup() {
Serial.begin(9600);
myBluetooth.begin(9600);

setupMotors();
setupHorn();
}

void loop() {
  if (myBluetooth.available() > 0){
    command = myBluetooth.read();
    Serial.println("Command recieved: " + String(command));

    switch(command){
        case 'F': moveForward(); break;
        case 'B': moveBackward(); break;
        case 'L': turnLeft(); break;
        case 'R': turnRight();  break;
        case 'S': stop(); break;
        case 'V': hornOn(); break;
        case 'v': hornOff(); break;
        }
    }
}