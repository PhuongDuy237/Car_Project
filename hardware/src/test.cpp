// #include "control.h"
// #include <Arduino.h>
// #include <SoftwareSerial.h>

// String command;

// void setup() {
//     Serial.begin(9600);
//     setupMotors();

//     Serial.println("Motor Test");
//     Serial.println("Commands:");
//     Serial.println("forward");
//     Serial.println("backward");
//     Serial.println("left");
//     Serial.println("right");
//     Serial.println("stop");
// }

// void loop() {
//     if (Serial.available()) {
//         command = Serial.readStringUntil('\n');
//         command.trim();   // Remove newline and spaces

//         if (command == "F") {
//             moveForward();      // Replace with your function
//             Serial.println("Moving forward");
//         }
//         else if (command == "B") {
//             moveBackward();     // Replace with your function
//             Serial.println("Moving backward");
//         }
//         else if (command == "L") {
//             turnLeft();         // Replace with your function
//             Serial.println("Turning left");
//         }
//         else if (command == "R") {
//             turnRight();        // Replace with your function
//             Serial.println("Turning right");
//         }
//         else if (command == "S") {
//             stop();       // Replace with your function
//             Serial.println("Stopped");
//         }
//         else {
//             Serial.print("Unknown command: ");
//             Serial.println(command);
//         }
//     }
// }