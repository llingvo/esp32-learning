# ESP32 MQTT Touch Demo

This demo connects ESP32 to a WiFi network and publishes touch sensor events to an MQTT broker.

## Features

- WiFi connectivity
- MQTT publish on touch sensor state changes
- Touch state debouncing
- Serial debug output

## Configuration

Before uploading, update WiFi and MQTT credentials in [src/main.cpp](src/main.cpp#L8-L14):

```cpp
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

const char* mqtt_server = "192.168.31.168";
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/touch";
```

## Build and Upload

1. Open this folder in VS Code with PlatformIO extension.
2. Select environment: `esp32doit-devkit-v1`.
3. Update WiFi/MQTT credentials in `src/main.cpp`.
4. Build and upload: `pio run -t upload`.
5. Open serial monitor at `115200` baud.

Expected log after boot:
```
WiFi connected
Attempting MQTT connection...
connected
Touch value: 650
```

## MQTT Topic

- **Topic:** `esp32/touch`
- **Payload:** `0` (not touched) or `1` (touched)
- **Threshold:** Touch value < 600 = touched state

## Hardware

- GPIO4 (T0): Touch sensor input
- Serial TX/RX for debugging

## Dependencies

- [PubSubClient](https://github.com/knolleary/pubsub_client) — MQTT client library

## Security Note

For production deployments:
- Use environment variables or config files instead of hardcoded credentials.
- Consider using TLS/SSL for MQTT connections.
- Implement credential rotation.
