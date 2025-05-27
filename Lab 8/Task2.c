#include <Arduino_LPS22HB.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BARO.begin()) {
    Serial.println("Error al iniciar el sensor de presión");
    while (1);
  }
}

void loop() {
  float presion = BARO.readPressure();  // en hPa

  float altitud = 44330 * (1.0 - pow(presion / 1013.25, 0.1903));

  Serial.print("Presión: ");
  Serial.print(presion);
  Serial.print(" hPa\tAltitud estimada: ");
  Serial.print(altitud);
  Serial.println(" m");

  delay(1000);
}
