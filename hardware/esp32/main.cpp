#include <Arduino.h>
#include <WiFi.h>
#include "car_control.h"
#include "arm_control.h"

const char* wifi = "";
const char* password = "";
const int hornFreq = 2600;

WiFiServer server(80); // open port 80
CarController* car = new CarController();

int speedLv = 1;
char command;

void setup() {
    Serial.begin(115200);
    car->setupCar();

    Serial.print("Connecting to Wifi");
    WiFi.begin(wifi, password);

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    Serial.print("Connected! The IP of Car is: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
    WiFiClient client = server.available();

    if(client){
        if(client.available() > 0){
            command = client.read();
            Serial.println("Command received: " + String(command));

            switch(command){
                // Speed control
                case 'P': speedLv = 0; car->stop(); break;
                case '1': speedLv = 1; break;
                case '2': speedLv = 2; break;
                case '3': speedLv = 3; break;
                case '4': speedLv = 4; break;

                // Direction control
                case 'F': car->moveForward(speedLv); break;
                case 'B': car->moveBackward(speedLv); break;
                case 'L': car->turnLeft(speedLv); break;
                case 'R': car->turnRight(speedLv);  break;
                case 'S': car->stop(); break;

                // Horn control
                case 'V': car->hornOn(hornFreq); break;
                case 'v': car->hornOff(); break;
            }
        }
    }
}