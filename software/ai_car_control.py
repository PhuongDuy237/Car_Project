import cv2
import mediapipe as mp
import math
import serial
import time

# Connect to board
ser = serial.Serial('COM13', 9600)
time.sleep(2)

# Initializing recognition
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands = 1, min_detection_confidence = 0.7)
mp_draw = mp.solutions.drawing_utils

# Turn on camera
cap = cv2.VideoCapture(0)

# Flag to avoid spam intruction
current_state = "S"

while True:
    success, img = cap.read() # Read 1 image frame from camera

    # Preprocessing
    img = cv2.flip(img, 1) # mirror picture
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB) # Change from BGR color to RGB color

    # Load image into mediapipe
    results = hands.process(imgRGB)

    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:
            # Draw backbone stick
            mp_draw.draw_landmarks(img, handLms, mp_hands.HAND_CONNECTIONS)

            # get height - width of image frame
            h, w, c = img.shape

            # get coordinates of wrist (0) and top of middle finger (12)
            # multifly with h, w to get the pixel ratio
            wx, wy = int(handLms.landmark[0].x * w), int(handLms.landmark[0].y * h) # wrist
            mx, my = int(handLms.landmark[12].x * w), int(handLms.landmark[12].y * h) # middle finger

            # Calculate the distance between wrist and middle finger
            hand_open_dist = math.hypot(wx - mx, wy - my)

            dx = mx - wx # hand lean left/right
            dy = wy - my # hand lean up/down

            # Logic controller
            command = 'S'
            status_text = "DUNG"
            color = (0, 0, 255)

            # Hold hand -> stop car
            if hand_open_dist < 80:
                command = 'S'
                status_text = "DUNG (PHANH)"
                color = (0, 0, 255)

            # Not hold hand -> move car
            else:
                if dy < -20:
                    command = 'B'
                    status_text = "DI LUI"
                    color = (0, 165, 255)
                elif dx < -30:
                    command = 'L'
                    status_text = "XOAY TRAI"
                    color = (255, 255, 0)
                elif dx > 30:
                    command = 'R'
                    status_text = "XOAY PHAI"
                    color = (255, 0, 255)
                else: # hand splaying and up
                    command = 'F'
                    status_text = "DI TOI"
                    color = (0,255,0)

            # Print text on screen
            cv2.putText(img, status_text, (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)

            # Avoid spam
            if current_state != command:
                print(f"Gui lenh {command} - {status_text}")
                ser.write(command.encode()) # Load command to board
                current_state = command

    # Show image frame
    cv2.imshow("AI Car Control", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()