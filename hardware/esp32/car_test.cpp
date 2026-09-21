// #include <Arduino.h>
// #include <WiFi.h>
// #include "car_control.h"

// const char* wifi = "Dep Trai";
// const char* password = "123456798";
// const int hornFreq = 2600;

// WiFiServer server(80); // open port 80
// CarController* car = new CarController();

// int speedLv = 1;
// char command;
// char currentDirection = 'S';

// void setup() {
//     Serial.begin(115200);
//     car->setupCar();

//     Serial.print("Connecting to Wifi");
//     WiFi.begin(wifi, password);

//     while (WiFi.status() != WL_CONNECTED){
//         delay(500);
//         Serial.print(".");
//     }

//     Serial.print("Connected! The IP of Car is: ");
//     Serial.println(WiFi.localIP());

//     WiFi.setSleep(false);

//     server.begin();
// }

// void loop() {
//     int distance = car->getDistance();
//     if (distance <= 15 && currentDirection == 'F'){
//         car->stop();
//         currentDirection = 'S';
//     }

//     WiFiClient client = server.available();

//     if(client){
//         client.setNoDelay(true);

//         if(client.available() > 0){
//             String request = client.readStringUntil('\r');
//             client.flush();

//             int slashIndex = request.indexOf('/');
//             if (slashIndex != -1 && request.length() > slashIndex + 1) {
//                 command = request.charAt(slashIndex + 1);
//             }
//             Serial.println("Command received: " + String(command));

//             switch(command){
//                 // Speed control
//                 case 'P': speedLv = 0; car->stop(); currentDirection = 'S'; break;
//                 case '1':
//                 case '2':
//                 case '3':
//                 case '4':
//                     speedLv = command - '0';

//                     if (currentDirection == 'F')    car->moveForward(speedLv);
//                     else if (currentDirection == 'B')   car->moveBackward(speedLv);
//                     else if (currentDirection == 'L')   car->turnLeft(speedLv);
//                     else if (currentDirection == 'R')   car->turnRight(speedLv);
//                     break;

//                 // Direction control
//                 case 'F':
//                     if (distance <= 15)     car->stop();

//                     else {
//                         car->moveForward(speedLv);
//                         currentDirection = 'F';
//                      }
//                      break;

//                 case 'B':
//                     car->moveBackward(speedLv);
//                     currentDirection = 'B';
//                     break;

//                 case 'L':
//                     car->turnLeft(speedLv);
//                     currentDirection = 'L';
//                     break;

//                 case 'R':
//                     car->turnRight(speedLv);
//                     currentDirection = 'R';
//                     break;

//                 case 'S':
//                 case '0':
//                     car->stop();
//                     currentDirection = 'S';
//                     break;

//                 // Horn control
//                 case 'V': car->hornOn(hornFreq); break;
//                 case 'v': car->hornOff(); break;
//             }
//         }

//         client.println("HTTP/1.1 200 OK");
//         client.println("Content-Type: text/plain");
//         client.println("Connection: close");
//         client.println();
//         client.stop();
//     }
// }
