#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ================= BLE CONFIG (Khớp với App Android) =================
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// ================= PIN CONFIG (Giữ nguyên logic của bạn) =================
int pin[10] = {14, 27, 26, 25, 4, 12, 13, 18, 19, 2};

// 4 Chân PWM điều khiển tốc độ
int pwm1 = 16;
int pwm2 = 17;
int pwm3 = 5;
int pwm4 = 23;

// ================= LOGIC MẢNG TRẠNG THÁI (Giữ nguyên từ file .h) =================
int forward[10]       = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1};
int backward[10]      = {0, 1, 0, 1, 1, 1, 0, 1, 0, 1};
int left[10]          = {0, 1, 1, 0, 1, 0, 1, 1, 0, 1};
int right[10]         = {1, 0, 0, 1, 1, 1, 0, 0, 1, 1};

int forwardLeft[10]   = {0, 0, 1, 0, 1, 0, 1, 0, 0, 1};
int forwardRight[10]  = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1};
int backwardLeft[10]  = {0, 1, 0, 0, 1, 0, 0, 1, 0, 1};
int backwardRight[10] = {0, 0, 0, 1, 1, 1, 0, 0, 0, 1};

int turnLeft[10]      = {0, 0, 1, 0, 1, 1, 0, 0, 0, 1};
int turnRight[10]     = {1, 0, 0, 0, 1, 0, 0, 1, 0, 1};

// ================= HÀM ĐIỀU KHIỂN MOTOR =================
void applyLogic(int logicArray[], int speed) {
  // 1. Nạp 10 trạng thái chân Digital
  for (int i = 0; i < 10; i++) {
    digitalWrite(pin[i], logicArray[i]);
  }
  
  // 2. Nạp tốc độ PWM (Chuyển đổi 0-100 sang 0-255)
  int pwmVal = map(speed, 0, 100, 0, 255);
  analogWrite(pwm1, pwmVal);
  analogWrite(pwm2, pwmVal);
  analogWrite(pwm3, pwmVal);
  analogWrite(pwm4, pwmVal);
}

void stopCar() {
  int stopLogic[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  applyLogic(stopLogic, 0);
}

// ================= XỬ LÝ DỮ LIỆU NHẬN TỪ APP =================
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("App đã kết nối!");
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      stopCar();
      Serial.println("App đã ngắt kết nối! Dừng xe.");
      pServer->getAdvertising()->start(); // Bắt đầu quảng bá lại để App tìm thấy
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        String data = String(rxValue.c_str());
        
        // Phân tích cú pháp chuỗi "CODE,SPEED" (Ví dụ: "F,75")
        int commaIndex = data.indexOf(',');
        if (commaIndex == -1) return;

        String code = data.substring(0, commaIndex);
        int speed = data.substring(commaIndex + 1).toInt();

        Serial.print("Lệnh nhận được: "); Serial.print(code);
        Serial.print(" | Tốc độ: "); Serial.println(speed);

        if (code == "F")       applyLogic(forward, speed);
        else if (code == "B")  applyLogic(backward, speed);
        else if (code == "L")  applyLogic(left, speed);
        else if (code == "R")  applyLogic(right, speed);
        else if (code == "FL") applyLogic(forwardLeft, speed);
        else if (code == "FR") applyLogic(forwardRight, speed);
        else if (code == "BL") applyLogic(backwardLeft, speed);
        else if (code == "BR") applyLogic(backwardRight, speed);
        else if (code == "TL") applyLogic(turnLeft, speed);
        else if (code == "TR") applyLogic(turnRight, speed);
        else                   stopCar();
      }
    }
};

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  // Khởi tạo các chân Output
  for (int i = 0; i < 10; i++) {
    pinMode(pin[i], OUTPUT);
  }
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(pwm4, OUTPUT);

  stopCar();

  // Khởi tạo BLE
  BLEDevice::init("ESP32_Car_BLE"); // Tên hiển thị khi App quét Bluetooth
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  // Bắt đầu phát tín hiệu Bluetooth
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Hệ thống BLE đã sẵn sàng! Hãy mở App và kết nối.");
}

void loop() {
  // Không cần xử lý gì trong loop, BLE dùng cơ chế Callback (ngắt)
  delay(10); 
}