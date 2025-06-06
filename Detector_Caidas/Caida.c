
#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

BLEService detectFall("e962c59e-3fd7-11f0-9fe2-0242ac120002");
BLEBoolCharacteristic fall(
  "e962c59e-3fd7-11f0-9fe2-0242ac120002",
  BLERead | BLENotify
);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Starting...");

  if (!IMU.begin()) {
    Serial.println("Fail initializing IMU!");
    while (1);
  }

  if (!BLE.begin()) {
    Serial.println("Fail initializing BLE!");
    while (1);
  }

  BLE.setLocalName("DetectFalls");
  detectFall.addCharacteristic(fall);
  BLE.addService(detectFall);

  fall.writeValue(false);
  BLE.advertise();

  Serial.println("Waiting for connection...");
}

void loop() {
  BLEDevice central = BLE.central();

  while (central.connected()) {
    if (IMU.accelerationAvailable()) {
      float x, y, z;
      IMU.readAcceleration(x, y, z);

      float result = sqrt((x * x) + (y * y) + (z * z));

      // WHEN THE FALL IS DETECTED
      if (esCaida(result)){
          fall.writeValue(true);
          delay(3000);
      }

      Serial.print(result);
      Serial.println('\t');

    }

    // WHEN THE FALL IS OVER
    fall.writeValue(false);
  } 
}
bool esCaida(float result) {          
  return result > 1.2;
}
