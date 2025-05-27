#include <Arduino_HTS221.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!HTS.begin()) {
    Serial.println("Error al iniciar el sensor");
    while (1);
  }
}

void loop() {
  float temperatura = HTS.readTemperature();
  float humedad = HTS.readHumidity();

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" ºC\tHumedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  delay(500);  // medio segundo entre lecturas
}
