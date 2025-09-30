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
// Estado da moto
// ========================
String motoId = "moto-001";
String statusMoto = "fora_patio";  

// ========================
// Setup inicial
// ========================
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi conectado!");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  Serial.println("📡 Dispositivo SUBSCRIBER iniciado!");
}

// ========================
// Reconexão ao broker MQTT
// ========================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("MotthruSubscriber")) {
      Serial.println(" conectado!");
      // Inscrever nos tópicos
      client.subscribe("motthru/portal");
      client.subscribe("motthru/dock");
      client.subscribe("motthru/manutencao");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando em 5s");
      delay(5000);
    }
  }
}

// ========================
// Callback para mensagens
// ========================
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("📩 [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(msg);

  if (strcmp(topic, "motthru/portal") == 0) {
    statusMoto = (msg == "entrada") ? "no_patio" : "fora_patio";
  } else if (strcmp(topic, "motthru/dock") == 0) {
    // apenas exibe
  } else if (strcmp(topic, "motthru/manutencao") == 0) {
    statusMoto = (msg == "manutencao") ? "em_manutencao" : "pronta";
  }

  Serial.print("📊 Status atual da ");
  Serial.print(motoId);
  Serial.print(": ");
  Serial.println(statusMoto);
  Serial.println("---------------------------------");
}

// ========================
// Loop principal
// ========================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
