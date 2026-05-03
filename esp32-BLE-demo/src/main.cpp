#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ----------------------
// UUID 定义
// ----------------------
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHAR_CONTROL_UUID   "abcd1234-1234-1234-1234-1234567890ab" // 手机写入
#define CHAR_STATE_UUID     "abcd5678-1234-1234-1234-1234567890ab" // ESP32 通知

BLECharacteristic *pStateCharacteristic;

// ----------------------
// 全局状态
// ----------------------
bool deviceConnected = false;

// ----------------------
// 控制 Characteristic 回调
// ----------------------
class ControlCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (!value.empty()) {
      Serial.print("Received command: ");
      Serial.println(value.c_str());

      // 这里可以解析 JSON 命令，例如 {"speed":50,"turn":-10}
      // 推荐使用 ArduinoJson 库解析
    }
  }
};

// ----------------------
// Server 回调
// ----------------------
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
    // 自动重新广播
    BLEDevice::startAdvertising();
  }
};

// ----------------------
// setup
// ----------------------
void setup() {
  Serial.begin(115200);

  // 初始化 BLE
  BLEDevice::init("ESP32_Robot");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // 创建 Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // 创建 Characteristic：控制命令
  BLECharacteristic *pControlCharacteristic = pService->createCharacteristic(
      CHAR_CONTROL_UUID,
      BLECharacteristic::PROPERTY_WRITE
  );
  pControlCharacteristic->setCallbacks(new ControlCallback());

  // 创建 Characteristic：状态通知
  pStateCharacteristic = pService->createCharacteristic(
      CHAR_STATE_UUID,
      BLECharacteristic::PROPERTY_NOTIFY
  );
  pStateCharacteristic->addDescriptor(new BLE2902());

  // 启动 Service
  pService->start();

  // 开始广播
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE advertising started");

  // 初始化触摸引脚 D4（T0）
  pinMode(4, INPUT);
}

// ----------------------
// 读取触摸状态并发送
// ----------------------
void sendState() {
  int touchValue = touchRead(4); // D4对应T0

  // 构造 JSON 字符串
  String json = "{\"touch\":" + String(touchValue) + "}";

  // 推送给手机
  pStateCharacteristic->setValue(json.c_str());
  pStateCharacteristic->notify();

  Serial.println("Sent state: " + json); // 可选，调试用
}

// ----------------------
// loop
// ----------------------
void loop() {
  if (deviceConnected) {
    sendState();
  }
  delay(200); // 每200ms发送一次
}
