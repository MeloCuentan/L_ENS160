// Controlar nivel de CO2

#ifndef L_ENS160_H
#define L_ENS160_H

#include <Wire.h>

#define L_ENS160_I2C_ADDRESS 0x53     // Dirección I2C por defecto del ENS160
#define L_ENS160_PART_ID 0x00         // Registro de identificación del sensor
#define L_ENS160_OPMODE 0x10          // Registro de modo de operación
#define L_ENS160_MODE_DEEP_SLEEP 0x00 // Modo de bajo consumo
#define L_ENS160_MODE_IDLE 0x01       // Modo inactivo
#define L_ENS160_MODE_STANDARD 0x02   // Modo estándar de operación
#define L_ENS160_MODE_RESET 0xF0      // Realizar un reset al sensor
#define L_ENS160_TEMP_IN 0x13         // Registro de temperatura interna
#define L_ENS160_RH_IN 0x15           // Registro de humedad interna
#define L_ENS160_ECO2 0x24            // Registro de concentración de CO2

class L_ENS160
{
public:
  L_ENS160(TwoWire &wirePort = Wire);             // Constructor
  bool begin();                                 // Inicialización del sensor
  uint16_t readCO2();                           // Lectura de la concentración de CO2
  void ambienteExterior(float temp, float hum); // Lectura de la temperatura y humedad exterior

private:
  TwoWire *_i2cPort;       // Puerto I2C
  uint8_t _address = 0x53; // Dirección I2C del sensor ENS160
  union dataUnion // Unión para almacenar los datos de 16 bits
  {                       
    uint16_t value_uint16; // Valor de 16 bits
    uint8_t bytes[2];      // Bytes de 8 bits
  };
  void temperaturaExterior(float temp); // Lectura de la temperatura exterior
  void humedadExterior(float hum);      // Lectura de la humedad exterior

  uint8_t readRegister(uint8_t reg);                            // Lectura de un registro
  void writeRegister(uint8_t reg, uint8_t value);               // Escritura de un registro
  void readRegisters(uint8_t reg, uint8_t *buffer, size_t len); // Lectura de varios registros
};

#endif
