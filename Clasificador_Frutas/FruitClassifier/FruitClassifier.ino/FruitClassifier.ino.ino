#include <TensorFlowLite.h>
#include <Arduino_APDS9960.h>
#include "model.h"

#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// CLASES DE FRUTA — DEBEN COINCIDIR CON Google Colab
const char* CLASSES[] = {
  "limon",
  "manzana",
  "zanahoria"
};
#define NUM_CLASSES (sizeof(CLASSES) / sizeof(CLASSES[0]))

// TensorFlowLite
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;

  constexpr int kTensorArenaSize = 8 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Inicializar sensor de color
  if (!APDS.begin()) {
    Serial.println("Error inicializando APDS9960.");
    while (1);
  }

  // Inicializar TensorFlow
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(modelo);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report("Modelo incompatible.");
    while (1);
  }

  static tflite::ops::micro::AllOpsResolver resolver;

  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, error_reporter);

  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    error_reporter->Report("Fallo al asignar tensores.");
    while (1);
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("Clasificador de frutas listo.");
}

void loop() {
  if (APDS.proximityAvailable() && APDS.readProximity() < 50 && APDS.colorAvailable()) {
    int r, g, b;
    APDS.readColor(r, g, b);
    float total = r + g + b;
    if (total == 0) return;

    // Normalizar RGB
    input->data.f[0] = r / total;
    input->data.f[1] = g / total;
    input->data.f[2] = b / total;

    // Ejecutar inferencia
    if (interpreter->Invoke() != kTfLiteOk) {
      Serial.println("Error en inferencia.");
      return;
    }

    // Mostrar resultados
    Serial.println("Resultado:");
    for (int i = 0; i < NUM_CLASSES; i++) {
      Serial.print(CLASSES[i]);
      Serial.print(": ");
      Serial.println(output->data.f[i], 3);
    }

    // Esperar a que se aleje el objeto
    while (APDS.readProximity() < 50) {
      delay(200);
    }

    Serial.println("------");
  }
}
