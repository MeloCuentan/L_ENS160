#include <Arduino.h>
#include <Wire.h>
#include "ENS160.h"

ENS160 sensor;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    sensor.begin();

    Serial.println("Inicializando ENS160...");
}

void loop()
{
    uint16_t co2 = sensor.readCO2();

    if (co2 == 0)
    {
        Serial.println("Esperando estabilización del sensor...");
    }
    else
    {
        Serial.print("eCO2: ");
        Serial.print(co2);
        Serial.println(" ppm");
    }

    delay(500); // Simula llamadas frecuentes al método
}
