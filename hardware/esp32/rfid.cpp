// #include <U8g2lib.h>
// #include <Wire.h>
// #include <rdm6300.h>
// #include <WiFi.h>
// #include <WebServer.h>
// #include <Preferences.h>

// // ==== Cấu hình chân ====
// #define RDM6300_RX_PIN 16
// #define BUTTON_PIN     4
// #define SDA_PIN        33
// #define SCL_PIN        32

// // ==== Ngưỡng thời gian ====
// #define TAG_AWAY_TIMEOUT_MS 1000
// #define DEBOUNCE_DELAY_MS   50
// #define FACE_DURATION_MS    1500
// #define HOLD_THRESHOLD_MS   500
// #define SCROLL_INTERVAL_MS  500
// #define MAX_TAGS 5
// #define VISIBLE_ROWS 4

// // ==== WiFi AP cho dashboard ====
// const char* AP_SSID = "TagFinder-Setup";
// const char* AP_PASS = "12345678";

// U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
// Rdm6300 rdm6300;
// WebServer server(80);
// Preferences prefs;

// // ==== Dữ liệu tag ====
// struct TagEntry {
//   uint32_t id = 0;
//   String name = "";
// };
// TagEntry tags[MAX_TAGS];
// unsigned long lastSeen[MAX_TAGS] = {0};
// int tagCount = 0;

// // ==== Trạng thái màn hình ====
// bool showStatusScreen = false;
// int scrollOffset = 0;

// // ==== Trạng thái nút bấm ====
// bool buttonState = HIGH;
// bool lastButtonReading = HIGH;
// unsigned long lastDebounceTime = 0;
// unsigned long pressStartTime = 0;
// bool holdTriggered = false;
// unsigned long lastScrollTime = 0;

// // ==== Cảm xúc động ====
// enum FaceState { FACE_HAPPY, FACE_BLINK, FACE_LOVE, FACE_SURPRISED, FACE_SAD, FACE_NEUTRAL };
// const int NUM_FACES = 6;
// unsigned long lastFaceChange = 0;
// int currentFace = FACE_HAPPY;

// // ==== Kiểu hiệu ứng icon: 0=mặt cảm xúc, 1=icon tĩnh, 2=icon nhấp nháy ====
// int iconStyle = 0;

// // ================= LƯU / ĐỌC CẤU HÌNH =================
// void loadConfig() {
//   prefs.begin("tagcfg", true);
//   tagCount = prefs.getInt("cnt", 0);
//   for (int i = 0; i < tagCount; i++) {
//     tags[i].id = prefs.getULong(("id" + String(i)).c_str(), 0);
//     tags[i].name = prefs.getString(("nm" + String(i)).c_str(), "Tag " + String(i + 1));
//   }
//   iconStyle = prefs.getInt("style", 0);
//   prefs.end();
// }

// void saveConfig() {
//   prefs.begin("tagcfg", false);
//   prefs.putInt("cnt", tagCount);
//   for (int i = 0; i < tagCount; i++) {
//     prefs.putULong(("id" + String(i)).c_str(), tags[i].id);
//     prefs.putString(("nm" + String(i)).c_str(), tags[i].name);
//   }
//   prefs.putInt("style", iconStyle);
//   prefs.end();
// }

// int findOrAddTag(uint32_t id) {
//   for (int i = 0; i < tagCount; i++) {
//     if (tags[i].id == id) return i;
//   }
//   if (tagCount < MAX_TAGS) {
//     tags[tagCount].id = id;
//     tags[tagCount].name = "Tag moi " + String(tagCount + 1);
//     tagCount++;
//     saveConfig();
//     return tagCount - 1;
//   }
//   return -1;
// }

// // ================= WEB DASHBOARD =================
// void handleRoot() {
//   String html = "<html><head><meta charset='utf-8'>"
//                  "<title>Tag Finder Dashboard</title></head><body>"
//                  "<h2>Danh sach tag</h2><form action='/save' method='POST'>";

//   for (int i = 0; i < tagCount; i++) {
//     html += String(i + 1) + ". ID: " + String(tags[i].id) + "<br>";
//     html += "Ten: <input name='name" + String(i) + "' value='" + tags[i].name + "'> ";
//     html += "<a href='/delete?idx=" + String(i) + "' onclick='return confirm(\"Xoa tag nay?\")'>[Xoa]</a><br><br>";
//   }
//   if (tagCount == 0) {
//     html += "<i>Chua co tag nao duoc quet. Dua tag vao gan RDM6300 de them.</i><br><br>";
//   }

//   html += "Kieu hieu ung icon mac dinh: <select name='style'>";
//   html += String("<option value='0'") + (iconStyle == 0 ? " selected" : "") + ">Khuon mat cam xuc dong</option>";
//   html += String("<option value='1'") + (iconStyle == 1 ? " selected" : "") + ">Icon tinh (ngon tay)</option>";
//   html += String("<option value='2'") + (iconStyle == 2 ? " selected" : "") + ">Icon nhap nhay</option>";
//   html += "</select><br><br>";

//   html += "<button type='submit'>Luu ten / kieu icon</button></form></body></html>";
//   server.send(200, "text/html", html);
// }

// void handleSave() {
//   for (int i = 0; i < tagCount; i++) {
//     if (server.hasArg("name" + String(i))) {
//       tags[i].name = server.arg("name" + String(i));
//     }
//   }
//   if (server.hasArg("style")) {
//     iconStyle = server.arg("style").toInt();
//   }
//   saveConfig();
//   server.sendHeader("Location", "/");
//   server.send(303);
// }

// void handleDelete() {
//   if (server.hasArg("idx")) {
//     int idx = server.arg("idx").toInt();
//     if (idx >= 0 && idx < tagCount) {
//       for (int i = idx; i < tagCount - 1; i++) {
//         tags[i] = tags[i + 1];
//         lastSeen[i] = lastSeen[i + 1];
//       }
//       tagCount--;
//       saveConfig();
//     }
//   }
//   server.sendHeader("Location", "/");
//   server.send(303);
// }

// void setupWebServer() {
//   WiFi.softAP(AP_SSID, AP_PASS);
//   Serial.print("Dashboard IP: ");
//   Serial.println(WiFi.softAPIP());

//   server.on("/", handleRoot);
//   server.on("/save", HTTP_POST, handleSave);
//   server.on("/delete", handleDelete);
//   server.begin();
// }

// // ================= CÁC BỘ PHẬN KHUÔN MẶT =================
// void drawEyesNormal() { u8g2.drawDisc(48, 26, 5); u8g2.drawDisc(80, 26, 5); }
// void drawEyesClosed() { u8g2.drawBox(43, 25, 10, 2); u8g2.drawBox(75, 25, 10, 2); }
// void drawEyesWide()   { u8g2.drawDisc(48, 26, 7); u8g2.drawDisc(80, 26, 7); }
// void drawEyesSad()    { u8g2.drawDisc(48, 28, 5); u8g2.drawDisc(80, 28, 5); u8g2.drawBox(46,34,2,5); u8g2.drawBox(82,34,2,5); }
// void drawCheeks()     { u8g2.drawCircle(38, 36, 4); u8g2.drawCircle(90, 36, 4); }
// void drawMouthSmile() { u8g2.drawLine(50,42,58,48); u8g2.drawLine(58,48,70,48); u8g2.drawLine(70,48,78,42); }
// void drawMouthSad()   { u8g2.drawLine(50,48,58,42); u8g2.drawLine(58,42,70,42); u8g2.drawLine(70,42,78,48); }
// void drawMouthOpen()  { u8g2.drawRBox(58, 42, 12, 10, 3); }
// void drawMouthNeutral(){ u8g2.drawBox(54, 44, 20, 2); }

// void drawFace(int state) {
//   u8g2.clearBuffer();
//   u8g2.setDrawColor(1);
//   switch (state) {
//     case FACE_HAPPY: drawEyesClosed(); drawMouthSmile(); drawCheeks(); break;
//     case FACE_BLINK: drawEyesClosed(); drawMouthNeutral(); break;
//     case FACE_LOVE: drawEyesNormal(); drawMouthSmile(); drawCheeks(); break;
//     case FACE_SURPRISED: drawEyesWide(); drawMouthOpen(); break;
//     case FACE_SAD: drawEyesSad(); drawMouthSad(); break;
//     case FACE_NEUTRAL: drawEyesNormal(); drawMouthNeutral(); break;
//   }
//   u8g2.sendBuffer();
// }

// void drawFaceAnimated() {
//   if (millis() - lastFaceChange > FACE_DURATION_MS) {
//     lastFaceChange = millis();
//     currentFace = (currentFace + 1) % NUM_FACES;
//   }
//   drawFace(currentFace);
// }

// // ================= ICON NGÓN TAY TĨNH =================
// void drawFingerIcon() {
//   u8g2.clearBuffer();
//   u8g2.setDrawColor(1);
//   u8g2.drawBox(58, 4, 12, 24);
//   u8g2.drawBox(48, 26, 8, 12);
//   u8g2.drawBox(72, 26, 8, 12);
//   u8g2.drawBox(40, 30, 48, 8);
//   u8g2.drawBox(26, 44, 14, 14);
//   u8g2.drawRBox(38, 40, 48, 22, 4);
//   u8g2.setDrawColor(0);
//   u8g2.drawBox(54, 46, 6, 10);
//   u8g2.drawBox(68, 46, 6, 10);
//   u8g2.setDrawColor(1);
//   u8g2.sendBuffer();
// }

// // ================= ICON NHẤP NHÁY =================
// void drawBlinkingIcon() {
//   if ((millis() / 500) % 2 == 0) {
//     drawFingerIcon();
//   } else {
//     u8g2.clearBuffer();
//     u8g2.sendBuffer();
//   }
// }

// // ================= DISPATCH MÀN HÌNH MẶC ĐỊNH =================
// void drawDefaultScreen() {
//   switch (iconStyle) {
//     case 0: drawFaceAnimated(); break;
//     case 1: drawFingerIcon(); break;
//     case 2: drawBlinkingIcon(); break;
//   }
// }

// // ================= ICON TICK / CROSS =================
// void drawCheck(int x, int y) { u8g2.drawLine(x, y+4, x+3, y+7); u8g2.drawLine(x+3, y+7, x+9, y); }
// void drawCross(int x, int y) { u8g2.drawLine(x, y, x+8, y+8); u8g2.drawLine(x, y+8, x+8, y); }

// // ================= MÀN HÌNH TRẠNG THÁI (CÓ SCROLL) =================
// void drawStatusScreen() {
//   if (tagCount > 0 && scrollOffset >= tagCount) scrollOffset = 0;

//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB08_tr);

//   if (tagCount == 0) {
//     u8g2.drawStr(0, 30, "Chua co tag nao");
//   } else {
//     int shown = 0;
//     for (int i = scrollOffset; i < tagCount && shown < VISIBLE_ROWS; i++, shown++) {
//       int y = 15 + shown * 15;
//       bool ok = (lastSeen[i] != 0) && (millis() - lastSeen[i] < TAG_AWAY_TIMEOUT_MS);
//       String label = String(i + 1) + ". " + tags[i].name;
//       u8g2.drawStr(0, y, label.c_str());
//       if (ok) drawCheck(105, y - 8); else drawCross(105, y - 8);
//     }
//   }
//   u8g2.sendBuffer();
// }

// // ================= SCROLL =================
// void scrollDown() {
//   scrollOffset++;
//   if (scrollOffset >= tagCount) {
//     scrollOffset = 0;
//   }
// }

// // ================= CẬP NHẬT TAG =================
// void updateTagStatus() {
//   uint32_t new_tag_id = rdm6300.get_new_tag_id();
//   if (new_tag_id) {
//     Serial.print("Tag moi: ");
//     Serial.println(new_tag_id);
//   }
//   uint32_t current_tag_id = rdm6300.get_tag_id();
//   if (current_tag_id) {
//     int idx = findOrAddTag(current_tag_id);
//     if (idx >= 0) lastSeen[idx] = millis();
//   }
// }

// // ================= NÚT BẤM (DEBOUNCE + NHẤN NGẮN / GIỮ ĐỂ SCROLL) =================
// void updateButton() {
//   bool reading = digitalRead(BUTTON_PIN);

//   if (reading != lastButtonReading) {
//     lastDebounceTime = millis();
//   }

//   if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY_MS) {
//     if (reading != buttonState) {
//       buttonState = reading;

//       if (buttonState == LOW) {
//         pressStartTime = millis();
//         holdTriggered = false;
//       } else {
//         if (!holdTriggered) {
//           showStatusScreen = !showStatusScreen;
//         }
//       }
//     }
//   }

//   if (buttonState == LOW && showStatusScreen && tagCount > VISIBLE_ROWS) {
//     if (!holdTriggered && (millis() - pressStartTime > HOLD_THRESHOLD_MS)) {
//       holdTriggered = true;
//       lastScrollTime = millis();
//       scrollDown();
//     } else if (holdTriggered && (millis() - lastScrollTime > SCROLL_INTERVAL_MS)) {
//       lastScrollTime = millis();
//       scrollDown();
//     }
//   }

//   lastButtonReading = reading;
// }

// // ================= SETUP =================
// void setup() {
//   Serial.begin(115200);
//   delay(300);

//   pinMode(BUTTON_PIN, INPUT_PULLUP);
//   Wire.begin(SDA_PIN, SCL_PIN);
//   u8g2.begin();

//   loadConfig();
//   setupWebServer();

//   rdm6300.begin(RDM6300_RX_PIN);
//   rdm6300.set_tag_timeout(300);

//   Serial.println("San sang doc the RDM6300...");
//   drawDefaultScreen();
// }

// // ================= LOOP =================
// void loop() {
//   server.handleClient();
//   updateTagStatus();
//   updateButton();

//   if (showStatusScreen) drawStatusScreen();
//   else drawDefaultScreen();

//   delay(30);
// }