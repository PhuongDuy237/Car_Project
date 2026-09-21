// #include <Arduino.h>
// #include <WiFi.h>
// #include "car_control.h"
// #include "arm_control.h"
// // ServoEasing.hpp chua phan cai dat day du, chi duoc include 1 lan duy nhat
// // trong ca project - dung o day (file co setup()/loop()) theo dung yeu cau
// // cua thu vien, de tranh loi "multiple definition" luc link.
// #include <ServoEasing.hpp>

// const char* wifi = "Uni Lounge";
// const char* password = "12345678";
// const int hornFreq = 2600;

// WiFiServer server(80); // open port 80
// CarController car;
// ArmController arm;

// int speedLv = 1;
// char command = 'S';
// char currentDirection = 'S';

// void setup() {
//     Serial.begin(115200);
//     car.setupCar();
//     arm.setupArm();

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
//     int distance = car.getDistance();
//     if (distance <= 15 && currentDirection == 'F'){
//         car.stop();
//         currentDirection = 'S';
//     }

//     WiFiClient client = server.available();

//     if (client){
//         client.setNoDelay(true);

//         // Chờ dữ liệu thực sự đến trước khi xử lý, tối đa 200ms
//         unsigned long waitStart = millis();
//         while (client.connected() && client.available() == 0){
//             if (millis() - waitStart > 200){
//                 break;
//             }
//         }

//         bool parsedOk = false;

//         if (client.available() > 0){
//             String request = client.readStringUntil('\r');
//             client.flush();

//             int slashIndex = request.indexOf('/');
//             if (slashIndex != -1 && request.length() > slashIndex + 1) {
//                 command = request.charAt(slashIndex + 1);
//                 parsedOk = true;
//             }
//         }

//         if (parsedOk){
//             Serial.println("Command received: " + String(command));

//             switch(command){
//                 // Speed control
//                 case 'P': speedLv = 0; car.stop(); currentDirection = 'S'; break;
//                 case '1':
//                 case '2':
//                 case '3':
//                 case '4':
//                     speedLv = command - '0';

//                     if (currentDirection == 'F')    car.moveForward(speedLv);
//                     else if (currentDirection == 'B')   car.moveBackward(speedLv);
//                     else if (currentDirection == 'L')   car.turnLeft(speedLv);
//                     else if (currentDirection == 'R')   car.turnRight(speedLv);
//                     break;

//                 // Direction control
//                 case 'F':
//                     if (distance <= 15)     car.stop();
//                     else {
//                         car.moveForward(speedLv);
//                         currentDirection = 'F';
//                     }
//                     break;

//                 case 'B':
//                     car.moveBackward(speedLv);
//                     currentDirection = 'B';
//                     break;

//                 case 'L':
//                     car.turnLeft(speedLv);
//                     currentDirection = 'L';
//                     break;

//                 case 'R':
//                     car.turnRight(speedLv);
//                     currentDirection = 'R';
//                     break;

//                 case 'S':
//                 case '0':
//                     car.stop();
//                     currentDirection = 'S';
//                     break;

//                 // Horn control
//                 case 'V': car.hornOn(hornFreq); break;
//                 case 'v': car.hornOff(); break;

//                 // Arm control - press and hold to keep rotating, release sends 's' to stop
//                 case 'A': arm.moveJointToLimit(0, 1); break;
//                 case 'D': arm.moveJointToLimit(0, -1); break;

//                 case 'W': arm.moveJointToLimit(1, -1); break;
//                 case 'X': arm.moveJointToLimit(1, 1); break;

//                 case 'T': arm.moveJointToLimit(2, 1); break;
//                 case 'G': arm.moveJointToLimit(2, -1); break;

//                 case 'J': arm.moveJointToLimit(3, -1); break;
//                 case 'K': arm.moveJointToLimit(3, 1); break;

//                 case 's': arm.stopAll(); break;
//             }
//         }

//         client.println("HTTP/1.1 200 OK");
//         client.println("Content-Type: text/plain");
//         client.println("Connection: close");
//         client.println();
//         client.stop();
//     }
// }