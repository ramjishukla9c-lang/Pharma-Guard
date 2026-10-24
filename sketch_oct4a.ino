#include <Adafruit_TinyUSB.h>
#include <bluefruit.h>

// BLE service and characteristic UUIDs
BLEService testService("12345678-1234-1234-1234-123456789abc");

BLECharacteristic testCharacteristic(
  "abcdefab-1234-1234-1234-abcdefabcdef"
);

unsigned long lastReading = 0;

void setup() {
  Serial.begin(115200);
  delay(5000);

  // Start BLE peripheral
  Bluefruit.begin();
  Bluefruit.setName("MED001");

  // Start service
  testService.begin();

  // Configure characteristic for reading and notifications
  testCharacteristic.setProperties(
    CHR_PROPS_READ | CHR_PROPS_NOTIFY
  );

  testCharacteristic.setPermission(
    SECMODE_OPEN,
    SECMODE_OPEN
  );

  testCharacteristic.begin();

  // Initial characteristic value
  testCharacteristic.write("MED001");

  // Advertise device name and service
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.addService(testService);
  Bluefruit.Advertising.start();

  Serial.println("BLE server started");
  Serial.println("Advertising as MED001");
  Serial.println("Sampling interval: 5 seconds");
}

void loop() {
  // Take a reading every 5 seconds
  if (millis() - lastReading >= 5000) {
    lastReading = millis();

    // Mock temperature between 5.00 and 6.00 °C
    float temperature = 18.0 + (random(0, 701) / 100.0);

    // Temporary elapsed-time timestamp
    uint32_t timestamp = millis();

    // Format: deviceID,timestamp,temperature
    char data[64];

    snprintf(
      data,
      sizeof(data),
      "MED001,%lu,%.2f",
      (unsigned long)timestamp,
      temperature
    );

    // Update BLE characteristic and notify connected clients
    testCharacteristic.write(data);
    testCharacteristic.notify(data);

    Serial.print("Reading sent: ");
    Serial.println(data);
  }
}
