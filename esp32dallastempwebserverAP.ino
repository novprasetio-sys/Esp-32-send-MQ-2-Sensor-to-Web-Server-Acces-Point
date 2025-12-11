#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Inisialisasi pin
const int ds_pin = 2;
OneWire oneWire(ds_pin);
DallasTemperature sensors(&oneWire);

// Inisialisasi Access Point
const char* ssid = "ESP32-AP";
const char* password = "12345678";

WebServer server(80);

void handleRoot() {
  float temperature = getTemperature();
  String html = "<html><body>";
  html += "<h1>ESP32 DS18B20 Temperature Sensor</h1>";
  html += "<p>Temperature: " + String(temperature) + " C</p>";
  html += "<form action='/download'>";
  html += "<button type='submit'>Download CSV</button>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleDownload() {
  float temperature = getTemperature();
  String csv_data = "Temperature\n";
  csv_data += String(temperature) + "\n";
  server.send(200, "text/csv", csv_data);
  server.sendHeader("Content-Disposition", "attachment; filename='temperature.csv'");
}

float getTemperature() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  return temperature;
}

void setup() {
  Serial.begin(115200);
  sensors.begin();

  // Inisialisasi Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Inisialisasi server
  server.on("/", handleRoot);
  server.on("/download", handleDownload);
  server.begin();
}

void loop() {
  server.handleClient();
  delay(1000);
}