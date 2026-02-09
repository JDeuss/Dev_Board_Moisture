#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Fill IN";                 // Your WiFi name
const char* password = "Fill IN";             // Password

// MQTT broker
const char* mqtt_server = "Fill IN";          // IP address of your Raspberry Pi
const int mqtt_port = 1883;
const char* mqtt_topic = "sensor/calibration";

// Sensor pin
const int sensorPin = A0;

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected, IP:");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "ESP32C3-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  Serial.println("Calibration mode started.");
  Serial.println("Move the sensor between AIR and WATER to read raw values.");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int rawValue = analogRead(sensorPin);

  Serial.print("Raw sensor value: ");
  Serial.println(rawValue);

  // Convert to char[] for MQTT
  char msg[10];
  itoa(rawValue, msg, 10);

  client.publish(mqtt_topic, msg);

  delay(2000);  // slow down for readability
}
