#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

Servo myServo;

const char* ssid = "Detonator1";
const char* password = "coaielemele";
WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "ESP32 Web Server");
}

void handleGpio26On() {
  myServo.write(90);
  server.send(200, "text/plain", "Servo on GPIO 26 moved to 90 degrees");
  delay(3000);
  handleGpio26Off();
}

void handleGpio26Off() {
  myServo.write(0); 
  server.send(200, "text/plain", "Servo on GPIO 26 moved to 0 degrees");
}

void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0); 
  myServo.setPeriodHertz(50);
  myServo.attach(26, 500, 2400);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/26/on", handleGpio26On);
  server.begin();
}

void loop() {
  server.handleClient();
}