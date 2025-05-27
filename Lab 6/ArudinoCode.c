#include <Arduino_LSM9DS1.h>

void setup() {
  Serial.begin(250000);
  while (!Serial);
  if (!IMU.begin()) {
    Serial.println("IMU no detectado");
    while (1);
  }
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'r') {
      float x, y, z;
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        Serial.print(x); Serial.print('\t');
        Serial.print(y); Serial.print('\t');
        Serial.println(z);
      }
    }
  }
}
