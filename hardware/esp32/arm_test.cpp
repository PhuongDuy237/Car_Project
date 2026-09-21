// #include <Arduino.h>
// #include <WiFi.h>
// #include "arm_control.h"

// const char* wifi = "Dep Trai";
// const char* password = "123456798";

// WiFiServer server(80); // open port 80
// ArmController* arm = new ArmController();

// char command;

// void setup() {
//     Serial.begin(115200);
//     arm->setupArm();

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
//     arm->update(); // advances any joint currently held down, one step at a time

//     WiFiClient client = server.available();

//     if(client){
//         client.setNoDelay(true);

//         // Client is accepted (TCP handshake done) but its HTTP request bytes may
//         // not have arrived yet. Give it a short, bounded wait instead of dropping
//         // the connection immediately - otherwise commands sent while the button is
//         // held get silently lost, which is what was causing the jerky motion.
//         unsigned long waitStart = millis();
//         while (client.connected() && !client.available() && millis() - waitStart < 15){
//             delay(1);
//         }

//         if(client.available() > 0){
//             String request = client.readStringUntil('\r');
//             client.flush();

//             int slashIndex = request.indexOf('/');
//             if (slashIndex != -1 && request.length() > slashIndex + 1) {
//                 command = request.charAt(slashIndex + 1);
//             }
//             Serial.println("Command received: " + String(command));

//             switch(command){
//                 // Arm control - press and hold to keep rotating, release sends 's' to stop
//                 case 'A': arm->startMove(0, 1); break;
//                 case 'D': arm->startMove(0, -1); break;

//                 case 'W': arm->startMove(1, -1); break;
//                 case 'X': arm->startMove(1, 1); break;

//                 case 'T': arm->startMove(2, 1); break;
//                 case 'G': arm->startMove(2, -1); break;

//                 case 'J': arm->startMove(3, -1); break;
//                 case 'K': arm->startMove(3, 1); break;

//                 case 's': arm->stopAll(); break;
//             }
//         }

//         client.println("HTTP/1.1 200 OK");
//         client.println("Content-Type: text/plain");
//         client.println("Connection: close");
//         client.println();
//         client.stop();
//     }
// }
