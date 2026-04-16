#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "MPU6050.h"

/* === NRF === */
RF24 radio(9,10);
const byte address[6] = "00001";

struct Data_Package {
  float yaw; //xoay
  float pitch; //gat
  bool isReset; // Co bao hieu reset
};
Data_Package data;

/* === MPU === */
MPU6050 mpu;

float yaw = 0, pitch = 0, yaw0 = 0;
unsigned long lastTime = 0;
unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 50; 
unsigned long sendCount = 0; 

void setup() {
  Serial.begin(9600);
  delay(2000);

  Wire.begin();
  Wire.setClock(10000);

  mpu.initialize();
  mpu.setSleepEnabled(false);

  if (!mpu.getDeviceID()) {
    Serial.println("MPU FAIL");
    while (1);
  }

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);   
  radio.stopListening();

  // --- LOGIC GỬI TÍN HIỆU RESET KHI MỚI KHỞI ĐỘNG ---
  Serial.println("Sending Reset Signal to Receiver...");
  data.isReset = true;
  for(int i = 0; i < 10; i++) { // Gửi 10 lần để đảm bảo bộ nhận nhận được
    radio.write(&data, sizeof(data));
    delay(10);
  }
  data.isReset = false; // Trả về false để bắt đầu truyền dữ liệu bình thường
  // ----------------------------------------------

  lastTime = millis();
  lastSend = millis();
  Serial.println("MPU RAW MODE OK");
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  if (dt > 0.1) dt = 0.01;


  float accpitch  = atan2(-ax, az) * 180 / M_PI;
  pitch += (gy / 131.0) * dt;
  pitch = pitch * 0.98 + accpitch * 0.02;

  yaw0 += (gx / 131.0) * dt;


  // GỬI DỮ LIỆU 
  // data.yaw = yaw ;
  // data.pitch = pitch;
  // data.isReset = false;

  if (now - lastSend >= SEND_INTERVAL) {
    lastSend = now;
    sendCount++; 
    yaw = yaw0 + 0.29 * sendCount;
      data.yaw = yaw ;
      data.pitch = pitch;
      data.isReset = false;
    radio.write(&data, sizeof(data));
    Serial.print("yaw: "); Serial.print(yaw);
    Serial.print(" pitch: "); Serial.println(pitch);
  }
}