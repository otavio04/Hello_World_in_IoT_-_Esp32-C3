// ===== CONFIGURAÇÃO MQTT =====
const brokerUrl = "wss://broker.hivemq.com:8884/mqtt";
const topic = "otavio/esp32/led";

// Client ID único
const clientId = "web_" + Math.random().toString(16).substr(2, 8);

// Conectar ao broker
const client = mqtt.connect(brokerUrl, {
  clientId: clientId,
  clean: true,
  connectTimeout: 4000
});

// Eventos MQTT
client.on("connect", () => {
  document.getElementById("status").innerText =
    "Status: conectado ao MQTT";
  console.log("Conectado ao broker MQTT");
});

client.on("error", (err) => {
  console.error("Erro MQTT:", err);
  document.getElementById("status").innerText =
    "Status: erro na conexão";
});

// Funções chamadas pelos botões
function ligar() {
  client.publish(topic, "ON");
}

function desligar() {
  client.publish(topic, "OFF");
}
