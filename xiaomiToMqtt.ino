#include <WiFi.h>
#include <PubSubClient.h>
#include "BLEDevice.h"

// WiFi & MQTT Server Credentials
const char *ssid = "ssid";
const char *password = "pass";
const char *mqtt_server = "server"; 
const char *mqtt_user = "mosquitto";  // No password here
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Xiaomi LYWSD03MMC BLE Service & Characteristic UUIDs
static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
static BLEUUID charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

BLEClient* pClient;
BLERemoteCharacteristic* pRemoteCharacteristic;

// Xiaomi Sensor MAC Address
const char* sensorMAC = "a4:c1:38:f5:9e:d4";

// Variables to store sensor data
float temperature = 0.0;
float humidity = 0.0;
float voltage = 0.0;
bool connected = false;

// MQTT Topics
const char* topicTemp = "homeassistant/xiaomi/temperature";
const char* topicHum = "homeassistant/xiaomi/humidity";
const char* topicVolt = "homeassistant/xiaomi/voltage";

// Timeout to check if data is received
unsigned long lastDataTime = 0;
const unsigned long reconnectTimeout = 10000;  // 10 seconds

// 🔹 MQTT Connection Function
void connectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("🔗 Connecting to MQTT Broker... ");
    if (mqttClient.connect("ESP32_Client", mqtt_user, "")) {  
      Serial.println("✅ Connected to MQTT!");
    } else {
      Serial.print("❌ MQTT Failed, error code: ");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}

// 🔹 Callback Function for BLE Notifications
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  
  temperature = (pData[0] | (pData[1] << 8)) * 0.01;  // Convert bytes to temperature
  humidity = pData[2];  // Humidity in %
  voltage = (pData[3] | (pData[4] << 8)) * 0.001;  // Convert bytes to voltage

  // Check if data is valid
  if (temperature != 0.0 && humidity != 0.0 && voltage != 0.0) {
    lastDataTime = millis();  // Update last data received time
    Serial.printf("🌡 Temperature: %.1f °C | 💧 Humidity: %.1f %% | 🔋 Voltage: %.3f V\n", 
                  temperature, humidity, voltage);

    // 🔹 Publish data to MQTT
    if (mqttClient.connected()) {
      mqttClient.publish(topicTemp, String(temperature).c_str(), true);
      mqttClient.publish(topicHum, String(humidity).c_str(), true);
      mqttClient.publish(topicVolt, String(voltage).c_str(), true);
      // Serial.println("📤 Data sent to MQTT!");
    }
  } else {
    Serial.println("❌ Invalid sensor data received, will attempt to reconnect.");
  }
}

// 🔹 Connect to BLE Sensor
bool connectToSensor() {
  Serial.print("🔗 Connecting to: ");
  Serial.println(sensorMAC);

  pClient = BLEDevice::createClient();
  
  if (!pClient->connect(BLEAddress(String(sensorMAC)))) {
    Serial.println("❌ Failed to connect to sensor!");
    return false;
  }

  Serial.println("✅ Connected to sensor!");

  // Get BLE Service
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (!pRemoteService) {
    Serial.println("❌ Failed to find service!");
    return false;
  }

  // Get BLE Characteristic
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (!pRemoteCharacteristic) {
    Serial.println("❌ Failed to find characteristic!");
    return false;
  }

  Serial.println("✅ Found characteristic!");

  // Register for Notifications
  pRemoteCharacteristic->registerForNotify(notifyCallback);

  connected = true;
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("🚀 Starting ESP32 BLE & MQTT Client...");

  // 🔹 Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");

  // 🔹 Connect to MQTT
  mqttClient.setServer(mqtt_server, mqttPort);  // Correct the server variable name here
  connectToMQTT();

  // 🔹 Initialize BLE
  BLEDevice::init("ESP32_BLE_Client");
  connectToSensor();
}

void loop() {
  if (!connected) {
    Serial.println("🔄 Reconnecting to Sensor...");
    if (millis() - lastDataTime > reconnectTimeout) {
      // Attempt to reconnect only if timeout is exceeded
      if (connectToSensor()) {
        Serial.println("✅ Reconnected to Sensor!");
      } else {
        Serial.println("❌ Failed to reconnect to Sensor!");
      }
      delay(5000);
    }
  } else {
    // Check if sensor is still connected
    if (!pClient->isConnected()) {
      Serial.println("❌ Sensor disconnected. Reconnecting...");
      connected = false; // Mark as disconnected
    }
  }

  if (!mqttClient.connected()) {
    connectToMQTT();
  }

  mqttClient.loop();  // Keep MQTT connection alive
  delay(2000);
}
