#include <Arduino.h>
#include <Wire.h>
#include "L_ENS160.h"

uint8_t aqi;
uint16_t tvoc;
uint16_t eco2;

L_ENS160 sensor;

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nIniciando ENS160...");

  Wire.begin();

  if (!sensor.begin()) {
    Serial.println("ERROR: Sensor no responde");
    delay(1000);
    while (1)
      ;
  }
  Serial.println("Sensor inicializado correctamente");

  sensor.setMode(L_ENS160_MODE_STANDARD);  // L_ENS160_MODE_DEEP_SLEEP, L_ENS160_MODE_IDLE, L_ENS160_MODE_STANDARD

  // Espera inicial para estabilización
  Serial.println("Esperando estabilización del sensor...");
  delay(1000);  // 1 segundo para calentamiento. Esto se puede suprimir
}

void loop() {
  static uint32_t lastPrint = 0;

  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();

    if (sensor.isDataReady()) {
      Serial.println("\n--- Informe de Calidad del Aire ---");

      // Configura condiciones ambientales con algún sensor externo como el AHT21
      // Esto hay que hacerlo para mejorar la presición de los valores leídos
      // sensor.ambienteExterior(25.0, 50.0); // tempMedida, humMedida

      // Lectura de valores
      aqi = sensor.readAQI();
      tvoc = sensor.readTVOC();
      eco2 = sensor.readCO2();

      // Los valores AQI van de 1 al 5 siendo 1 Exelente
      Serial.print("Calidad del Aire (AQI): ");
      Serial.print(aqi);
      Serial.print(" - ");
      Serial.println(getAQIDescription(aqi));

      Serial.print("Contaminantes Orgánicos (TVOC): ");
      Serial.print(tvoc);
      Serial.println(" ppb (Objetivo < 250 ppb)");

      Serial.print("CO2 Equivalente: ");
      Serial.print(eco2);
      Serial.println(" ppm (Óptimo < 600 ppm)");

      // Evaluación básica
      if (aqi <= 2 && tvoc < 250 && eco2 < 1000) {
        Serial.println(">> CALIDAD DEL AIRE: EXCELENTE <<");
      } else if (aqi <= 3) {
        Serial.println(">> CALIDAD DEL AIRE: ACEPTABLE <<");
      } else {
        Serial.println(">> CALIDAD DEL AIRE: MEJORAR VENTILACIÓN <<");
      }
    }
  }
}

String getAQIDescription(uint8_t aqi) {
  switch (aqi) {
    case 1: return "Excelente";
    case 2: return "Buena";
    case 3: return "Moderada";
    case 4: return "Pobre";
    case 5: return "Muy pobre";
    default: return "Desconocido";
  }
}
