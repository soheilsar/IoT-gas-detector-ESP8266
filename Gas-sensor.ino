#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int mq4Pin = A0;

const float Vcc = 5.0;
const int CALIBRATION_SAMPLES = 50;
const int READ_DELAY = 100;

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const String deviceId = "esp_ABC123";

const char* serverUrl = "https://870b-79-127-226-2.ngrok-free.app/gasvalue";

float Ro = 1.0;

WiFiClient client;
HTTPClient http;

float getPPM(float ratio) {
  float m = -0.318;
  float b = 0.77;
  float ppm_log = (log10(ratio) - b) / m;
  return pow(10, ppm_log);
}

float readRs() {
  int adc = analogRead(mq4Pin);
  float voltage = (adc / 1023.0) * Vcc;
  float rs = (Vcc - voltage) / voltage;
  return rs;
}

float calibrateRo() {
  float rs_avg = 0;
  Serial.println("Calibrating... Please ensure clean air.");
  delay(2000);

  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    rs_avg += readRs();
    delay(READ_DELAY);
  }

  rs_avg /= CALIBRATION_SAMPLES;
  float ro = rs_avg / 4.4;

  Serial.print("Calibration complete. Ro = ");
  Serial.println(ro);
  return ro;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected ✅");

  Ro = calibrateRo();
  Serial.println("Starting readings...\n");
}

void loop() {
  float rs = readRs();
  float ratio = rs / Ro;
  float ppm = getPPM(ratio);

  Serial.print("PPM: ");
  Serial.println(ppm, 1);

  if (WiFi.status() == WL_CONNECTED) {
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"deviceId\":\"" + deviceId + "\",\"value\":" + String(ppm, 1) + "}";

    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode > 0) {
      Serial.print("✅ Data sent. Response: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("❌ Error sending data: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("❌ WiFi disconnected");
  }

  delay(5000);  // every 5 seconds
}