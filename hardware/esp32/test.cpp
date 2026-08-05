#include <Arduino.h>
#include <WiFi.h>
#include "arm_control.h"

const char* wifi = "";
const char* password = "";

WiFiServer server(80);
ArmController* arm = new ArmController();

void setup() {
    Serial.begin(115200);
    arm->setupArm();

    Serial.print("Connecting to Wifi");
    WiFi.begin(wifi, password);

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    Serial.print("Connected! The IP of Arm is: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
    WiFiClient client = server.available();

    if (client.available() > 0){
        char command = client.read();
        Serial.println("Command recieved: " + String(command));

        switch(command){
            case 'A': 
            int curr = arm->getCurrentAngle(0);
            arm->moveJoint(0, curr--);
            break;

            case 'D':
            int curr = arm->getCurrentAngle(0);
            arm->moveJoint(0, curr++);
            break;

            case 'W':
            int curr = arm->getCurrentAngle(1);
            arm->moveJoint(1, curr++); break;

            case 'X':
            int curr = arm->getCurrentAngle(1);
            arm->moveJoint(1, curr--); break;

            case 'T':
            int curr = arm->getCurrentAngle(2);
            arm->moveJoint(2, curr++); break;

            case 'G':
            int curr = arm->getCurrentAngle(2);
            arm->moveJoint(2, curr--); break;

            case 'J':
            int curr = arm->getCurrentAngle(3);
            arm->moveJoint(3, curr++); break;

            case 'K':
            int curr = arm->getCurrentAngle(3);
            arm->moveJoint(3, curr--); break;
        }
    }
}