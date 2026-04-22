#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Nombre de tu red Wifi";
const char* password = "password de tu red Wifi";
const char* mqtt_server = "Direccion IP de tu broker MQTT (Raspberry Pi)";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
int contador = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("IP de ESP32: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");

    String clientId = "ESP32Publisher-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println(" conectado");
    } else {
      Serial.print(" falló, rc=");
      Serial.print(client.state());
      Serial.println(" reintentando en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;

    String mensaje = "Hola desde ESP32. Contador: " + String(contador++);
    Serial.print("Publicando mensaje: ");
    Serial.println(mensaje);

    client.publish("esp32/datos", mensaje.c_str());
  }
}