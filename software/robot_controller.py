import time

import cv2
import keyboard
import serial

from camera import CameraController, DISPLAY_SCALE

# ======== Cau hinh ========
SERIAL_PORT = 'COM13'
BAUD_RATE = 9600

# Uu tien khi giu nhieu phim dieu huong cung luc
MOVE_KEYS = [('w', 'F'), ('s', 'B'), ('a', 'L'), ('d', 'R')]

# ======== Ket noi Bluetooth toi xe ========
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
time.sleep(2)  # cho mach reset sau khi mo cong

camera = CameraController()
camera.start()

current_state = 'S'
prev_j = False
prev_r = False
prev_k = False

print("Dieu khien xe: giu w/a/s/d (chay ngay ca khi chua co camera).")
print("Chup anh: j. Quay/dung video: r. Thoat: q.")

try:
    while True:
        # --- Dieu khien xe: chi gui lenh khi trang thai doi, tranh spam ---
        command = 'S'
        for key, cmd in MOVE_KEYS:
            if keyboard.is_pressed(key):
                command = cmd
                break

        if command != current_state:
            ser.write(command.encode())
            current_state = command
            print(f"Gui lenh: {command}")

        # --- Chup anh (phim j) ---
        j_now = keyboard.is_pressed('j')
        if j_now and not prev_j:
            if camera.camera_ready:
                camera.request_photo()
            else:
                print("Chua co camera, khong the chup anh.")
        prev_j = j_now

        # --- Quay / dung video (phim r) ---
        r_now = keyboard.is_pressed('r')
        if r_now and not prev_r:
            if camera.camera_ready:
                camera.toggle_recording()
            else:
                print("Chua co camera, khong the quay video.")
        prev_r = r_now

        # --- Bat/tat flash (phim k) - chua ho tro ---
        k_now = keyboard.is_pressed('k')
        if k_now and not prev_k:
            print("Tinh nang bat/tat flash chua duoc ho tro.")
        prev_k = k_now

        # --- Hien thi hinh anh camera (neu co) ---
        frame_to_show = camera.get_frame()
        if frame_to_show is not None:
            display_frame = cv2.resize(frame_to_show, None, fx=DISPLAY_SCALE, fy=DISPLAY_SCALE)
            cv2.imshow("Robot Camera", display_frame)
        cv2.waitKey(1)

        if keyboard.is_pressed('q'):
            break

        time.sleep(0.02)

finally:
    camera.stop()
    ser.write(b'S')
    ser.close()
    cv2.destroyAllWindows()
