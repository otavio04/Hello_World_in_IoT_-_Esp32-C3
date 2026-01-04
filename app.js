const brokerUrl = "wss://broker.hivemq.com:8884/mqtt";

const topicCmd = "otavio/esp32/led/cmd";
const topicStatus = "otavio/esp32/led/status";

const clientId = "web_" + Math.random().toString(16).substr(2, 8);

const client = mqtt.connect(brokerUrl, {
  clientId: clientId,
  clean: true,
  connectTimeout: 4000
});

client.on("connect", () => {
  document.getElementById("status").innerText =
    "Status MQTT: conectado";

  client.subscribe(topicStatus);
});

client.on("message", (topic, message) => {
  if (topic === topicStatus) {
    const estado = message.toString();

    if (estado === "ON") {
      document.getElementById("status").innerText =
        "LED ligado (confirmado pelo ESP32)";
    } else {
      document.getElementById("status").innerText =
        "LED desligado (confirmado pelo ESP32)";
    }
  }
});

function ligar() {
  client.publish(topicCmd, "ON");
}

function desligar() {
  client.publish(topicCmd, "OFF");
}
