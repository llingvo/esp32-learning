# ESP32 BLE Demo

This demo creates an ESP32 BLE peripheral named `ESP32_Robot`.

It exposes:
- 1 Service
- 1 writable characteristic for control commands
- 1 notify characteristic for state updates (touch value)

## BLE UUIDs

- Service UUID: `12345678-1234-1234-1234-1234567890ab`
- Control Characteristic (Write): `abcd1234-1234-1234-1234-1234567890ab`
- State Characteristic (Notify): `abcd5678-1234-1234-1234-1234567890ab`

## Build and Upload

1. Open this folder in VS Code with PlatformIO extension.
2. Select environment: `esp32doit-devkit-v1`.
3. Build and upload.
4. Open serial monitor at `115200` baud.

Expected log after boot:
- `BLE advertising started`

## Test with nRF Connect (Mobile)

You can use nRF Connect app to connect and inspect characteristics.

1. Open nRF Connect and start scan.
2. Find device name `ESP32_Robot`.
3. Connect to device.
4. Expand the service with UUID `12345678-1234-1234-1234-1234567890ab`.
5. Verify two characteristics exist:
   - `abcd1234-1234-1234-1234-1234567890ab` (Write)
   - `abcd5678-1234-1234-1234-1234567890ab` (Notify)
6. Enable notifications on state characteristic (tap CCCD/Notify switch in nRF Connect).
7. Observe incoming values every 200 ms, example payload:
   - `{"touch":37}`
8. Write command to control characteristic (String/UTF-8), example:
   - `{"speed":50,"turn":-10}`

After writing, Serial Monitor prints:
- `Received command: ...`

## Notes

- Touch value comes from GPIO4 (`touchRead(4)`, T0).
- Notifications are sent only when BLE central is connected.
- If central disconnects, ESP32 starts advertising again automatically.
