#include <WiFi.h>
#include <PubSubClient.h>

// ========================
// Configuration
// ========================
// IMPORTANT: Update these with your WiFi and MQTT credentials
// For production, use environment variables or secure storage
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

const char* mqtt_server = "192.168.31.168";
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/touch";
const char* mqtt_client_id = "ESP32TouchClient";

WiFiClient espClient;
PubSubClient client(espClient);

const int touchPin = T0;  // 使用 ESP32 支持的触摸引脚
int lastTouchState = 0;

const int TOUCH_THRESHOLD = 600;

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  // 触摸引脚初始化不使用 interrupt
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read touch sensor
  int touchValue = touchRead(touchPin);
  Serial.print("Touch value: ");
  Serial.println(touchValue);
  
  // Detect touch state (lower value = touched)
  int touchState = touchValue < TOUCH_THRESHOLD ? 1 : 0;

  // Only publish on state change (debouncing)
  if (touchState != lastTouchState) {
    lastTouchState = touchState;
    const char* payload = touchState ? "1" : "0";
    client.publish(mqtt_topic, payload);
    Serial.print("Published touch state: ");
    Serial.println(payload);
  }

  delay(100);
}