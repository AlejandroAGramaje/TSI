#include <Arduino_APDS9960.h>

bool cabeceraImpresa = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error inicializando sensor APDS-9960");
    while (1);  // Bucle infinito si falla el sensor
  }

  Serial.println("Esperando proximidad para comenzar a capturar...");
}

void loop() {
  // Si el objeto está cerca
  if (APDS.proximityAvailable()) {
    int proximity = APDS.readProximity();
    
    if (proximity < 50) {  // Cuanto más bajo, más cerca (ajustable)
      
      // Si el sensor puede leer color
      if (APDS.colorAvailable()) {
        int r, g, b;
        APDS.readColor(r, g, b);

        // Imprimir cabecera solo una vez
        if (!cabeceraImpresa) {
          Serial.println("Red,Green,Blue");
          cabeceraImpresa = true;
        }

        // Normalizar
        float suma = r + g + b;
        float rn = r / suma;
        float gn = g / suma;
        float bn = b / suma;

        // Imprimir en formato CSV
        Serial.print(rn, 3); Serial.print(",");
        Serial.print(gn, 3); Serial.print(",");
        Serial.println(bn, 3);

        delay(200);  // Tiempo entre muestras (ajustable)
      }

    } else {
      cabeceraImpresa = false;  // Resetear para la próxima fruta
    }
  }
}
