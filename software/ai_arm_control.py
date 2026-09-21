import cv2
import mediapipe as mp
import math
import serial
import time

# Mở cổng Serial nối với mạch (Nhớ đổi 'COM3' thành cổng COM máy ông đang cắm mạch)
ser = serial.Serial('COM5', 9600) 
time.sleep(2) # Chờ mạch khởi động

# Khởi tạo bộ não MediaPipe Hand Tracking
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7)
mp_draw = mp.solutions.drawing_utils

cap = cv2.VideoCapture(0) # 0 là webcam mặc định

current_state = "M" 

while True:
    success, img = cap.read()
    if not success: continue

    img = cv2.flip(img, 1)
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(imgRGB)

    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:
            mp_draw.draw_landmarks(img, handLms, mp_hands.HAND_CONNECTIONS)
            h, w, c = img.shape
            tx, ty = int(handLms.landmark[4].x * w), int(handLms.landmark[4].y * h)
            ix, iy = int(handLms.landmark[8].x * w), int(handLms.landmark[8].y * h)
            
            distance = math.hypot(tx - ix, ty - iy)
            
            # Logic mới: Đã tăng khoảng cách lên 60 và chặn spam lệnh
            if distance < 60:
                cv2.putText(img, "Trang thai: GAP", (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
                # Chỉ bắn lệnh G nếu trước đó nó đang ở trạng thái M
                if current_state != "G":
                    ser.write(b'G') 
                    current_state = "G" # Cập nhật lại cờ
            else:
                cv2.putText(img, "Trang thai: MO", (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                # Chỉ bắn lệnh M nếu trước đó nó đang ở trạng thái G
                if current_state != "M":
                    ser.write(b'M') 
                    current_state = "M" # Cập nhật lại cờ

    cv2.imshow("AI Hand Control", img)
    if cv2.waitKey(1) & 0xFF == ord('q'): # Bấm phím 'q' để thoát
        break

cap.release()
cv2.destroyAllWindows()