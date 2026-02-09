#include <WiFi.h>
#include <PubSubClient.h>

// WLAN-Zugangsdaten
const char* ssid = "Fill IN";							//Your Wlan name
const char* password = "Fill IN";						//Password

// MQTT-Broker
const char* mqtt_server = "Fill IN"; 						//IP address of your Raspberry Pi
const int mqtt_port = 1883;
const char* mqtt_topic = "pflanze1";

//Sensor
const int sensorPin = A0;
										//The following values should be adjust after calibraation
const int airValue = 3000;   							// Sensor value in dry soil (or air)
const int waterValue = 1200; 							// Sensor value in wet soil (or water)

int sensorValue;
int moisturePercent;

//Deepb Sleep

const unsigned long SLEEP_DURATION_US = 5 * 60 * 1000000;   			//5 minutes

#define LED_BUILTIN 10    //Test


// MQTT-Client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Verbinde mit WLAN...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWLAN verbunden, IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Verbinde mit MQTT...");
    String clientId = "ESP32C3-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("verbunden");
    } else {
      Serial.print("Fehler, rc=");
      Serial.print(client.state());
      Serial.println(" versuche erneut in 5 Sekunden");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);


  pinMode(LED_BUILTIN, OUTPUT);     //Test
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  digitalWrite(LED_BUILTIN, HIGH);    //Test

  sensorValue = analogRead(sensorPin);
  sensorValue = constrain(sensorValue, waterValue, airValue);             		// Limit values to calibration range
  moisturePercent = map(sensorValue, airValue, waterValue, 0, 100);       		// Convert to percentage


  char msg[10];
  dtostrf(moisturePercent, 4, 1, msg);                                                 	//Convert float to char[] with 1 decimal place

  Serial.print("Sende Wert: ");
  Serial.println(moisturePercent);

  client.publish(mqtt_topic, msg);
  delay(60000);

  esp_sleep_enable_timer_wakeup(SLEEP_DURATION_US);                       
  Serial.println("Deep sleep");
  esp_deep_sleep_start();
}
