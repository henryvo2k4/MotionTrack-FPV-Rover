#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "web_control.h" 

// ================= AP CONFIG =================
const char* ap_ssid = "Remote_Controller"; 
const char* ap_pass = "12345678"; 

// ================= PIN CONFIG (Theo logic bạn cung cấp) =================
int pin[10] = {14, 27, 26, 25, 4, 12, 13, 18, 19, 2};

// 4 Chân PWM điều khiển tốc độ
int pwm1  = 16;
int pwm2 = 17;
int pwm3   = 5;
int pwm4  = 23;

int forward[10]       = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1};
int backward[10]      = {0, 1, 0, 1, 1, 1, 0, 1, 0, 1};
int left[10]          = {0, 1, 1, 0, 1, 0, 1, 1, 0, 1};
int right[10]         = {1, 0, 0, 1, 1, 1, 0, 0, 1, 1};

int forwardLeft[10]   = {0, 0, 1, 0, 1, 0, 1, 0, 0, 1};
int forwardRight[10]  = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1};
int backwardLeft[10]  = {0, 1, 0, 0, 1, 0, 0, 1, 0, 1};
int backwardRight[10] = {0, 0, 0, 1, 1, 1, 0, 0, 0, 1};

// ================= SERVER CONFIG =================
const byte DNS_PORT = 53;
WebServer server(80);
DNSServer dnsServer;

// ================= HÀM ĐIỀU KHIỂN MOTOR =================
void applyLogic(int logicArray[], int speed) {
  // 1. Nạp 10 trạng thái chân Digital
  for (int i = 0; i < 10; i++) {
    digitalWrite(pin[i], logicArray[i]);
  }
  
  // 2. Nạp tốc độ PWM (Chuyển đổi 0-100 sang 0-255)
  int pwmVal;
  if (speed>20) pwmVal = map(speed, 21, 100, 100, 255);
  else pwmVal = 0;
  analogWrite(pwm1, pwmVal);
  analogWrite(pwm2, pwmVal);
  analogWrite(pwm3, pwmVal);
  analogWrite(pwm4, pwmVal);

}

void stopCar() {
  int stopLogic[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  applyLogic(stopLogic, 0);
}

// ================= WEB HANDLER =================
void handleControl() {
  if (!server.hasArg("v")) {
    server.send(400, "text/plain", "Missing Arg");
    return;
  }

  String val = server.arg("v");
  int commaIndex = val.indexOf(',');
  if (commaIndex == -1) return;

  String code = val.substring(0, commaIndex);
  int speed = val.substring(commaIndex + 1).toInt();

  Serial.printf("[WEB] Code: %s | Speed: %d\n", code.c_str(), speed);

  if (code == "F")       applyLogic(forward, speed);
  else if (code == "B")  applyLogic(backward, speed);
  else if (code == "L")  applyLogic(left, speed);
  else if (code == "R")  applyLogic(right, speed);
  else if (code == "FL") applyLogic(forwardLeft, speed);
  else if (code == "FR") applyLogic(forwardRight, speed);
  else if (code == "BL") applyLogic(backwardLeft, speed);
  else if (code == "BR") applyLogic(backwardRight, speed);
  else stopCar();

  server.send(200, "text/plain", "OK");
}

void handleRoot() {
  server.send_P(200, "text/html", WEB_CONTROL_HTML);
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 10; i++) {
    pinMode(pin[i], OUTPUT);
  }
  
  // Cấu hình 4 chân PWM
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(pwm4, OUTPUT);

  stopCar();

  // Wifi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  // Captive Portal
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.on("/generate_204", handleRoot);
  server.on("/fwlink", handleRoot);
  server.on("/hotspot-detect.html", handleRoot);

  server.onNotFound([]() {
    server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
    server.send(302, "text/plain", "");
  });

  server.begin();
  Serial.println("ESP32 Mecanum Server Started!");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}