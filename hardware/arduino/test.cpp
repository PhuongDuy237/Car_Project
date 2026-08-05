#include <Arduino.h>
#include <SoftwareSerial.h>
#include "arm_control.h"

SoftwareSerial myBluetooth(2, 3);
char command;
const int step = 10;
ArmController* arm = new ArmController();

void setup() {
    Serial.begin(9600);
    myBluetooth.begin(9600);

    arm->setupArm();
}

void loop() {
    if (myBluetooth.available() > 0) {
        command = myBluetooth.read();
        Serial.println("Command recieved: " + String(command));

        switch(command) {
                // Các lệnh di chuyển (Khi nhấn giữ phím bên Python)
                case 'L': arm->moveJointToLimit(0, 1); break;
                case 'R': arm->moveJointToLimit(0, -1); break;
                case 'F': arm->moveJointToLimit(1, -1); break;
                case 'B': arm->moveJointToLimit(1, 1); break;
                case 'W': arm->moveJointToLimit(3, 1); break;
                case 'U': arm->moveJointToLimit(3, -1); break;
                
                // LỆNH PHANH GẤP (Khi nhả phím bên Python)
                case 'S': case 'w': case 'u': arm->stopAll(); break; 
            }
    } 
}