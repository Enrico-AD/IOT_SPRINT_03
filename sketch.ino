#include <WiFi.h>
#include <PubSubClient.h>

// ========================
// Configurações WiFi
// ========================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ========================
// Configurações MQTT
// ========================
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// ========================
// Pinos
// ========================
#define BTN_PORTAL 21
#define BTN_MANUTENCAO 19
#define LED_PATIO 5

// ========================
// Estado da moto
// ========================
String motoId = "moto-001";
String statusMoto = "fora_patio"; // inicial

// ========================
// Setup
// ========================
void setup() {
  Serial.begin(115200);

  pinMode(BTN_PORTAL, INPUT_PULLUP);
  pinMode(BTN_MANUTENCAO, INPUT_PULLUP);
  pinMode(LED_PATIO, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi conectado!");

  client.setServer(mqttServer, mqttPort);

  Serial.println("🚀 Dispositivo PUBLISHER iniciado!");
}

// ========================
// Reconexão MQTT
// ========================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("MottuPublisher")) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}

// ========================
// Publicar mensagem
// ========================
void publishStatus(const char* topic, const char* msg) {
  if (client.publish(topic, msg)) {
    Serial.print("📤 Publicado em ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(msg);
  } else {
    Serial.println("❌ Falha ao publicar");
  }
}

// ========================
// Loop principal
// ========================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Botão portal → entrada/saída
  if (digitalRead(BTN_PORTAL) == LOW) {
    if (statusMoto == "fora_patio") {
      statusMoto = "no_patio";
      publishStatus("mottu/portal", "entrada");
      digitalWrite(LED_PATIO, HIGH);
    } else {
      statusMoto = "fora_patio";
      publishStatus("mottu/portal", "saida");
      digitalWrite(LED_PATIO, LOW);
    }
    delay(500); // debounce
  }

  // Botão manutenção → alterna manutenção/pronta
  if (digitalRead(BTN_MANUTENCAO) == LOW) {
    if (statusMoto != "em_manutencao") {
      statusMoto = "em_manutencao";
      publishStatus("mottu/manutencao", "manutencao");
    } else {
      statusMoto = "pronta";
      publishStatus("mottu/manutencao", "pronta");
    }
    delay(500); // debounce
  }
}
