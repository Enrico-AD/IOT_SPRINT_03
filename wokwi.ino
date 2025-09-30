#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* serverName = "http://xxx.xxx.xx.x:1880/motthru"; // inserir o IP da sua máquina ao testar

const int trigPin = 5;
const int echoPin = 18;
const int ledPin = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-fi conectado!");
}

void loop() {
  long duration = readUltrasonicDistance();
  float distance = duration * 0.034 / 2;

  Serial.print("Distância: ");
  Serial.println(distance);

  if (distance < 30) {
    digitalWrite(ledPin, HIGH);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String payload = "{\"id_moto\":\"MOTO001\",\"status\":\"Detectado\",\"localizacao\":\"Manutenção - Pátio A\"}";

      int httpResponseCode = http.POST(payload);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.print("Erro no POST: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }

    delay(5000);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(500);
}

long readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH);
}
