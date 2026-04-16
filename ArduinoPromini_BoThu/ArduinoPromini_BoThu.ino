#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

/* ===== NRF24 ===== */
#define NRF_CE_PIN  9
#define NRF_CSN_PIN 10

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);
const byte address[6] = "00001";

/* ===== SERVOS ===== */
Servo panServo;
Servo tiltServo;

/* ===== DATA STRUCT ===== */
struct Data_Packet {
  float yaw;   
  float pitch;
  bool isReset; 
};

Data_Packet data;

/* ===== BIẾN ĐIỀU KHIỂN OFFSET ===== */
float initialYaw = 0;
float initialPitch = 0;
bool isFirstPacket = true; 

const int PAN_DEFAULT  = 90;  // Vị trí mặc định Servo Pan
const int TILT_DEFAULT = 90;  // Vị trí mặc định Servo Tilt

/* ===== TIMING ===== */
unsigned long lastRecvTime = 0;
unsigned long lastPrint    = 0;
const unsigned long RX_TIMEOUT   = 2000; 
const unsigned long PRINT_PERIOD = 300;  

/* ===== HÀM RESET HỆ THỐNG ===== */
void resetSystem() {
  radio.stopListening();
  digitalWrite(NRF_CE_PIN, LOW);
  delay(500);
  digitalWrite(NRF_CE_PIN, HIGH);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.startListening();
  
  // Đánh dấu để lấy lại mốc 0 khi nhận gói tin tiếp theo
  isFirstPacket = true; 
  panServo.write(PAN_DEFAULT);
  tiltServo.write(TILT_DEFAULT);
  Serial.println(">>> SYSTEM RESET: Waiting for new zero point...");
}

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("=== RECEIVER START ===");

  pinMode(NRF_CE_PIN, OUTPUT);

  if (radio.begin()) {
    Serial.println("NRF24L01 OK");
  } else {
    Serial.println("NRF24L01 FAIL");
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.startListening();

  panServo.attach(3);
  tiltServo.attach(5);

  // Đưa servo về vị trí trung tâm lúc khởi động
  panServo.write(PAN_DEFAULT);
  tiltServo.write(TILT_DEFAULT);

  lastRecvTime = millis();
}

void loop() {

  /* ===== NHẬN DỮ LIỆU ===== */
  if (radio.available()) {
    
    // Đọc gói tin mới nhất
    while (radio.available()) {
      radio.read(&data, sizeof(data));
    }

    lastRecvTime = millis();

    // 1. Kiểm tra lệnh Reset từ bộ gửi
    if (data.isReset) {
      resetSystem();
      return; 
    }

    // 2. Thiết lập mốc 0 nếu là gói tin đầu tiên sau khi Reset
    if (isFirstPacket) {
      initialYaw = data.yaw;
      initialPitch = data.pitch;
      isFirstPacket = false;
      Serial.print("Zero Point Set -> Yaw: "); Serial.print(initialYaw);
      Serial.print(" | Pitch: "); Serial.println(initialPitch);
    }

    // 3. Tính toán sai lệch (Offset)
    float yawOffset   = data.yaw - initialYaw;
    float pitchOffset = data.pitch - initialPitch;

    // 4. Chuyển đổi sang góc Servo
    int panAngle  = PAN_DEFAULT - (int)yawOffset;
    int tiltAngle = TILT_DEFAULT - (int)pitchOffset;

    // 5. Giới hạn góc quay để bảo vệ Servo
    panAngle  = constrain(panAngle, 0, 180);
    tiltAngle = constrain(tiltAngle, 30, 150); 

    panServo.write(panAngle);
    tiltServo.write(tiltAngle);

    Serial.println(data.yaw);
  }
  /* ===== CHỐNG TREO NRF ===== */
  if (millis() - lastRecvTime > RX_TIMEOUT) {
    Serial.println("NRF DISCONNECTED -> RESETTING...");
    resetSystem();
    lastRecvTime = millis();
  }
}