#include <WiFi.h>
#include <PubSubClient.h>

// ====== CONFIGURAÇÕES ======
const char* ssid = "brisa-994818";
const char* password = "vpiom1i6";

// Broker MQTT (HiveMQ público)
const char* mqtt_server = "broker.hivemq.com";

// Tópicos MQTT
const char* topicCmd = "otavio/esp32/led/cmd";
const char* topicStatus = "otavio/esp32/led/status";

// Pino do LED
const int ledPin = 8;

// ====== OBJETOS ======
WiFiClient espClient;
PubSubClient client(espClient);

// ====== FUNÇÃO CALLBACK ======
// É chamada automaticamente quando chega mensagem MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  if (String(topic) == topicCmd) {
    if (msg == "ON") {
      digitalWrite(LED_PIN, HIGH);
      client.publish(topicStatus, "ON");
    }
    else if (msg == "OFF") {
      digitalWrite(LED_PIN, LOW);
      client.publish(topicStatus, "OFF");
    }
  }
}

// ====== CONEXÃO WIFI ======
void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ====== CONEXÃO MQTT ======
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT... ");

    // Client ID (pode ser qualquer nome único)
    String clientId = "ESP32C3-Otavio";

    if (client.connect(clientId.c_str())) {
      Serial.println("conectado!");
      
      // Inscreve no tópico
      client.subscribe(topic);
      Serial.print("Inscrito no tópico: ");
      Serial.println(topic);
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}

// ====== SETUP ======
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  setupWiFi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ====== LOOP ======
void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop(); // mantém a conexão MQTT viva
}
