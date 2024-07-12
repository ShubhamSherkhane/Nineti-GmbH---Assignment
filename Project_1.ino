#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUIDs for the BLE Service and Characteristics
#define SERVICE_UUID        "00000002-0000-0000-FDFD-FDFDFDFDFDFD"
#define TEMP_CHAR_UUID      "12345678-1234-5678-1234-56789abcdef1"
#define HUMIDITY_CHAR_UUID  "12345678-1234-5678-1234-56789abcdef2"

// Mock functions to generate temperature and humidity values
float getTemperature() {
    // Simulate temperature value
    return 25.0 + (rand() % 100) / 10.0;  // Random temperature between 25.0 and 34.9
}

float getHumidity() {
    // Simulate humidity value
    return 50.0 + (rand() % 100) / 10.0;  // Random humidity between 50.0 and 59.9
}

BLECharacteristic *tempCharacteristic;
BLECharacteristic *humidityCharacteristic;

void setup() { Serial.begin(115200);

    // Initialize BLE
    BLEDevice::init("ESP32_BLE");

    // Create BLE Server
    BLEServer *pServer = BLEDevice::createServer();

    // Create BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristics for Temperature
    tempCharacteristic = pService->createCharacteristic(
                          TEMP_CHAR_UUID,
                          BLECharacteristic::PROPERTY_READ | 
                          BLECharacteristic::PROPERTY_NOTIFY
                        );
    tempCharacteristic->addDescriptor(new BLE2902());

    // Create BLE Characteristics for Humidity
    humidityCharacteristic = pService->createCharacteristic(
                              HUMIDITY_CHAR_UUID,
                              BLECharacteristic::PROPERTY_READ | 
                              BLECharacteristic::PROPERTY_NOTIFY
                            );
    humidityCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

void loop() {
    float temperature = getTemperature();
    float humidity = getHumidity();

    tempCharacteristic->setValue(temperature);
    humidityCharacteristic->setValue(humidity);

    tempCharacteristic->notify();
    humidityCharacteristic->notify();

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    delay(10000);
     }
