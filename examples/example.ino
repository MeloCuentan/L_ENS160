#include <Arduino.h>
#include <Wire.h>
#include "L_ENS160.h"

L_ENS160 sensor; // Creamos objeto

void setup()
{
  Serial.begin(115200); // Iniciar monitor serie
  Wire.begin();         // Iniciar bus I2C
  if (!sensor.begin())  // Si da error al iniciar Sensor
  {
    {
      Serial.println("Error al inicializar el sensor ENS160."); // Mostrar info
      while (1)                                                 // Bucle infinito
        ;
    }
    Serial.println("Inicializando ENS160..."); // Mostrar info
  }
}

void loop()
{
  uint16_t co2 = sensor.readCO2(); // Leer eCO2

  if (co2 == 0) // Si el valor leído es 0
  {
    Serial.println("Esperando estabilización del sensor..."); // Mostrar info
  }
  else // Si no...
  {
    Serial.print("eCO2: "); // Mostrar valor leído
    Serial.print(co2);
    Serial.println(" ppm");
  }

  delay(500); // Simula tiempo en terminar el resto del loop
}
