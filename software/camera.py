import os
import threading
import time
from datetime import datetime
from urllib.parse import quote

CAMERA_OPEN_TIMEOUT_MS = 5000  # thoi gian toi da cho mo stream truoc khi bo cuoc
CAMERA_READ_TIMEOUT_MS = 5000  # thoi gian toi da cho doc 1 frame truoc khi bo cuoc (mac dinh ffmpeg la 30000)

# Phai dat truoc khi cv2 mo bat ky stream nao thi FFmpeg moi ap dung dung timeout nay.
# cap.set(cv2.CAP_PROP_READ_TIMEOUT_MSEC, ...) khong dang tin cay tren moi build OpenCV/Windows.
os.environ.setdefault(
    "OPENCV_FFMPEG_CAPTURE_OPTIONS",
    f"timeout;{CAMERA_READ_TIMEOUT_MS * 1000}",  # ffmpeg 'timeout' tinh bang microgiay
)

import cv2

# ======== Cau hinh ========
CAMERA_HOST = '192.168.1.6:8081'
CAMERA_USERNAME = 'admin'
CAMERA_PASSWORD = '1234'
CAMERA_URL = f'http://{quote(CAMERA_USERNAME)}:{quote(CAMERA_PASSWORD)}@{CAMERA_HOST}/video'

IMAGES_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'images')
os.makedirs(IMAGES_DIR, exist_ok=True)

VIDEO_FPS = 20.0
CAMERA_RETRY_DELAY = 1.0  # giay, thoi gian cho giua cac lan thu ket noi lai camera
DISPLAY_SCALE = 0.5  # ty le thu nho khung hinh khi hien thi (anh chup/video van giu nguyen goc)


class CameraController:
    """Ket noi/ket noi lai camera dien thoai trong 1 thread rieng, khong lam
    khung tien trinh goi no. Dung get_frame() de lay frame moi nhat, va
    request_photo()/toggle_recording() de chup anh/quay video."""

    def __init__(self, camera_url=CAMERA_URL):
        self.camera_url = camera_url

        self._frame_lock = threading.Lock()
        self._latest_frame = None
        self.camera_ready = False

        self._photo_event = threading.Event()
        self._record_toggle_event = threading.Event()
        self._stop_event = threading.Event()

        self._thread = threading.Thread(target=self._worker, daemon=True)

    def start(self):
        self._thread.start()

    def stop(self):
        self._stop_event.set()
        self._thread.join(timeout=2)

    def get_frame(self):
        with self._frame_lock:
            return None if self._latest_frame is None else self._latest_frame.copy()

    def request_photo(self):
        self._photo_event.set()

    def toggle_recording(self):
        self._record_toggle_event.set()

    def _worker(self):
        cap = None
        video_writer = None
        recording = False
        fail_count = 0
        read_fail_count = 0
        got_first_frame = False

        while not self._stop_event.is_set():
            if cap is None or not cap.isOpened():
                cap = cv2.VideoCapture()
                cap.set(cv2.CAP_PROP_OPEN_TIMEOUT_MSEC, CAMERA_OPEN_TIMEOUT_MS)
                cap.set(cv2.CAP_PROP_READ_TIMEOUT_MSEC, CAMERA_READ_TIMEOUT_MS)
                cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)  # luon lay frame moi nhat, khong don backlog khi mang cham
                cap.open(self.camera_url, cv2.CAP_FFMPEG)
                if not cap.isOpened():
                    self.camera_ready = False
                    fail_count += 1
                    if fail_count == 1 or fail_count % 10 == 0:
                        print(f"Khong ket noi duoc camera tai {CAMERA_HOST} (lan thu {fail_count}).")
                        print("  - Kiem tra dien thoai va PC co cung mang Wifi khong.")
                        print("  - Kiem tra IP dien thoai hien tai (co the doi do DHCP), sua CAMERA_HOST neu can.")
                        print("  - Kiem tra app camera IP tren dien thoai da bat 'Start server' chua.")
                    time.sleep(CAMERA_RETRY_DELAY)
                    continue
                fail_count = 0
                print(f"Da mo duoc stream tai {CAMERA_HOST}, dang cho frame dau tien...")

            success, frame = cap.read()
            if not success:
                cap.release()
                cap = None
                self.camera_ready = False
                read_fail_count += 1
                if read_fail_count == 1 or read_fail_count % 10 == 0:
                    print(f"Mo duoc stream nhung doc frame that bai (lan {read_fail_count}), dang ket noi lai...")
                    print("  - Co the sai dinh dang URL/endpoint (vd /video), sai user/pass, hoac app camera doi codec.")
                time.sleep(CAMERA_RETRY_DELAY)
                continue
            read_fail_count = 0

            if not got_first_frame:
                got_first_frame = True
                h, w = frame.shape[:2]
                print(f"Da nhan frame dau tien tu camera ({w}x{h}).")

            self.camera_ready = True
            with self._frame_lock:
                self._latest_frame = frame

            if self._photo_event.is_set():
                filename = os.path.join(IMAGES_DIR, f"photo_{datetime.now().strftime('%Y%m%d_%H%M%S')}.jpg")
                cv2.imwrite(filename, frame)
                print(f"Da luu anh: {filename}")
                self._photo_event.clear()

            if self._record_toggle_event.is_set():
                if not recording:
                    h, w = frame.shape[:2]
                    filename = os.path.join(IMAGES_DIR, f"video_{datetime.now().strftime('%Y%m%d_%H%M%S')}.mp4")
                    video_writer = cv2.VideoWriter(filename, cv2.VideoWriter_fourcc(*'mp4v'), VIDEO_FPS, (w, h))
                    recording = True
                    print(f"Bat dau quay video: {filename}")
                else:
                    recording = False
                    video_writer.release()
                    video_writer = None
                    print("Da dung quay video.")
                self._record_toggle_event.clear()

            if recording and video_writer is not None:
                video_writer.write(frame)

        if video_writer is not None:
            video_writer.release()
        if cap is not None:
            cap.release()


def _standalone_main():
    """Chay rieng file nay de test ket noi camera dien thoai, khong can xe/Bluetooth."""
    import keyboard

    controller = CameraController()
    controller.start()

    prev_j = False
    prev_r = False

    print("Dang ket noi camera dien thoai. Chup anh: j. Quay/dung video: r. Thoat: q.")

    try:
        while True:
            frame = controller.get_frame()
            if frame is not None:
                display_frame = cv2.resize(frame, None, fx=DISPLAY_SCALE, fy=DISPLAY_SCALE)
                cv2.imshow("Robot Camera", display_frame)
            cv2.waitKey(1)

            j_now = keyboard.is_pressed('j')
            if j_now and not prev_j:
                if controller.camera_ready:
                    controller.request_photo()
                else:
                    print("Chua co camera, khong the chup anh.")
            prev_j = j_now

            r_now = keyboard.is_pressed('r')
            if r_now and not prev_r:
                if controller.camera_ready:
                    controller.toggle_recording()
                else:
                    print("Chua co camera, khong the quay video.")
            prev_r = r_now

            if keyboard.is_pressed('q'):
                break

            time.sleep(0.02)
    finally:
        controller.stop()
        cv2.destroyAllWindows()


if __name__ == '__main__':
    _standalone_main()
