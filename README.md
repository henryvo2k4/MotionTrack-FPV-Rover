# 🏎️ MotionTrack FPV Rover - Gesture-Controlled Surveillance Robot

[![English](https://img.shields.io/badge/Language-English-blue)](#-english-version) [![Tiếng Việt](https://img.shields.io/badge/Ngôn_ngữ-Tiếng_Việt-red)](#-bản-tiếng-việt)

---

## 🇬🇧 English Version

MotionTrack FPV Rover is an advanced mobile exploration platform featuring two independent but synchronized control systems: a smartphone-controlled Rover and a head/hand-tracked FPV Gimbal.

### 🚀 Key Technical Features
* **Hybrid Connectivity:** Uses **Bluetooth Low Energy (BLE)** for vehicle maneuvering and **NRF24L01 (2.4GHz RF)** for high-speed, low-latency gimbal synchronization.
* **Precision Motion Tracking:** The transmitter uses an **MPU6050 (IMU)** to calculate Yaw and Pitch. It implements a **Complementary Filter** to merge accelerometer and gyroscope data, providing stable, drift-free angles.
* **Smart Calibration:** Features an automatic **Zero Point Offset** system. Upon startup or reset signal, the receiver recalibrates the servos based on the current IMU orientation for perfect alignment.
* **Omnidirectional Movement:** The ESP32-based Rover processes complex drive logic (Forward, Backward, Lateral, Turn) received from a custom Android application.

### 📐 System Components
1. **The Rover (ESP32):** Receives BLE commands to drive 4 motors with PWM speed control.
2. **The Tracker (Arduino Pro Mini + MPU6050):** Transmits angular data at 50ms intervals.
3. **The Gimbal (Arduino Pro Mini + Servos):** Receives RF data and maps it to Pan/Tilt movements (0-180°).

---

## 🇻🇳 Bản Tiếng Việt

MotionTrack FPV Rover là nền tảng robot thám hiểm tiên tiến với hai hệ thống điều khiển độc lập: Xe trinh sát điều khiển qua App điện thoại và Gimbal camera tự động xoay theo chuyển động cơ thể.

### 🚀 Tính năng kỹ thuật nổi bật
* **Kết nối Hybrid:** Kết hợp **Bluetooth Low Energy (BLE)** để điều khiển hướng di chuyển của xe và **NRF24L01 (2.4GHz RF)** để đồng bộ hóa Gimbal với độ trễ cực thấp.
* **Theo dõi chuyển động chính xác:** Bộ phát sử dụng cảm biến **MPU6050**. Hệ thống áp dụng thuật toán **Lọc bù (Complementary Filter)** để kết hợp dữ liệu từ gia tốc kế và con quay hồi chuyển, giúp tính toán góc Yaw/Pitch ổn định, không bị trôi.
* **Cân chuẩn thông minh (Zero Point Offset):** Tích hợp cơ chế tự động thiết lập mốc 0. Khi khởi động hoặc nhận tín hiệu Reset, bộ thu sẽ tự động căn chỉnh lại góc Servo dựa trên vị trí hiện tại của cảm biến.
* **Điều hướng đa hướng:** Rover sử dụng ESP32 xử lý các tập lệnh phức tạp (Tiến, lùi, cua, đi ngang) được gửi từ ứng dụng Android tùy biến.

### ⚙️ Hướng dẫn cài đặt
1. **Rover:** Nạp `esp32_CarControl.ino` và cài đặt file `carcontrol.apk` lên điện thoại Android.
2. **Gimbal:** Nạp code cho bộ gửi (có MPU6050) và bộ nhận (có Servo). Đảm bảo địa chỉ NRF24L01 khớp nhau (`00001`).
3. **Sử dụng:** Kết nối Bluetooth với "ESP32_Car_BLE", đeo bộ phát vào tay/đầu và quan sát camera FPV xoay theo chuyển động của bạn.
