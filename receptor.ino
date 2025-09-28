#include <WiFi.h>
#include <PubSubClient.h>

// Configurações de rede WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configurações do broker MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis de status da moto
String motoId = "moto-001";
String statusMoto = "fora_patio";  
// Status possíveis: fora_patio, no_patio, em_manutencao, pronta

// ============================
// Setup inicial
// ============================
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  Serial.println("Dispositivo RECEPTOR de motos iniciado!");
}

// ============================
// Reconexão ao broker MQTT
// ============================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("ReceptorMottu")) {
      Serial.println("Conectado!");
      // Tópicos que o receptor escuta
      client.subscribe("mottu/portal");
      client.subscribe("mottu/dock");
      client.subscribe("mottu/manutencao");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

// ============================
// Callback para mensagens MQTT
// ============================
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Mensagem recebida no tópico [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  // Atualiza status com base no tópico
  if (strcmp(topic, "mottu/portal") == 0) {
    if (msg == "entrada") {
      statusMoto = "no_patio";
      Serial.println("🚪 Moto entrou no pátio.");
    } else if (msg == "saida") {
      statusMoto = "fora_patio";
      Serial.println("🚪 Moto saiu do pátio.");
    }

  } else if (strcmp(topic, "mottu/dock") == 0) {
    if (msg == "ocupada") {
      Serial.println("📍 Moto estacionada em uma vaga.");
    } else if (msg == "livre") {
      Serial.println("📍 Moto retirada da vaga.");
    }

  } else if (strcmp(topic, "mottu/manutencao") == 0) {
    if (msg == "manutencao") {
      statusMoto = "em_manutencao";
      Serial.println("🔧 Moto em manutenção.");
    } else if (msg == "pronta") {
      statusMoto = "pronta";
      Serial.println("✅ Moto pronta para uso.");
    }
  }

  // Exibe status atual
  Serial.print("📊 Status atual da ");
  Serial.print(motoId);
  Serial.print(": ");
  Serial.println(statusMoto);
  Serial.println("---------------------------------------");
}

// ============================
// Loop principal
// ============================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
