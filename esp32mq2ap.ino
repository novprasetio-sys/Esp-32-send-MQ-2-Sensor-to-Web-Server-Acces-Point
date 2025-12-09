// Definisikan pengaturan Access Point
const char *ssid = "ESP32_MQ2_Monitor"; // Nama AP
const char *password = "password123";  // Password AP (min. 8 karakter)

#include <WiFi.h>
#include <WebServer.h>

// Definisikan pengaturan Access Point
const char *ssid = "ESP32_MQ2_Monitor"; // Nama AP
const char *password = "password123";  // Password AP (min. 8 karakter)

// Pin Analog Sensor MQ-2
const int MQ2_PIN = 34; // Pin GPIO34 (ADC1_CH6)

// Buat objek WebServer di port 80
WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(MQ2_PIN, INPUT);

  // 1. Konfigurasi ESP32 sebagai Access Point
  Serial.print("Menyiapkan Access Point...");
  WiFi.softAP(ssid, password);
  
  // Ambil IP lokal yang ditetapkan untuk AP
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(IP);

  // 2. Routing Web Server
  server.on("/", handleRoot); // Ketika klien mengakses root ("/")
  server.onNotFound(handleNotFound); // Ketika klien mengakses halaman yang tidak ada

  // 3. Mulai Server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Proses permintaan klien
  delay(2);
}

// Fungsi untuk menangani permintaan pada root ("/")
void handleRoot() {
  // Baca data sensor analog (nilai 0 hingga 4095)
  int sensorValue = analogRead(MQ2_PIN);

  // Tentukan status berdasarkan nilai, ini hanya contoh sederhana
  String status;
  if (sensorValue > 2500) {
    status = " *BAHAYA!* Gas/Asap Terdeteksi Tinggi!";
  } else if (sensorValue > 1000) {
    status = "Perhatian: Gas/Asap Terdeteksi Sedang.";
  } else {
    status = "Aman: Tidak ada deteksi signifikan.";
  }

  // Isi Halaman HTML yang akan dikirim ke Browser
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><title>MQ-2 Web Monitor</title>";
  // Refresh otomatis setiap 5 detik
  html += "<meta http-equiv='refresh' content='5'>"; 
  html += "</head><body style='font-family: Arial; text-align: center;'>";
  
  html += "<h1>MQ-2 Gas/Smoke Sensor Monitor</h1>";
  html += "<p>Hubungkan ke jaringan: <b>" + String(ssid) + "</b></p>";
  
  html += "<hr>";

  // Tampilkan Nilai Mentah
  html += "<h2>Nilai Sensor Mentah (ADC)</h2>";
  html += "<h1 style='color: #007bff;'>" + String(sensorValue) + "</h1>"; 
  html += "<p>(Range: 0 - 4095)</p>";

  // Tampilkan Status
  html += "<h2>Status Deteksi</h2>";
  html += "<p style='padding: 15px; border: 2px solid #333; display: inline-block; background-color: #f8f9fa;'>" + status + "</p>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// Fungsi untuk halaman yang tidak ditemukan (404)
void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}
