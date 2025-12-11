#include <WiFi.h>
#include <WebServer.h>

// ==== GANTI SESUAI MAU ====
const char* ssid = "ESP32_ULTRASONIC";
const char* password = "12345678";

// ==== PIN ULTRASONIK ====
#define TRIG 5
#define ECHO 18

WebServer server(80);

// Hitung jarak cm
long getDistanceCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000); // timeout 30ms
  long distance = duration * 0.034 / 2;       // ke cm
  return distance;
}

// Halaman web
String htmlPage(long distance) {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta http-equiv='refresh' content='1'/>";   // Auto refresh tiap 1 detik
  html += "<title>ESP32 Ultrasonic</title>";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;padding-top:30px;}";
  html += "h1{color:#2196F3;}";
  html += "div{font-size:40px;margin-top:20px;}";
  html += "</style></head><body>";
  html += "<h1>ESP32 Ultrasonic Web Server</h1>";
  html += "<div>Distance: <b>" + String(distance) + " cm</b></div>";
  html += "</body></html>";
  return html;
}

void handleRoot() {
  long distance = getDistanceCM();
  server.send(200, "text/html", htmlPage(distance));
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.println("Mengaktifkan Access Point…");
  WiFi.softAP(ssid, password);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("WebServer berjalan…");
}

void loop() {
  server.handleClient();
}