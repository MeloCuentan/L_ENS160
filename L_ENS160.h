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
#define L_ENS160_STATUS 0x20          // Registro de estado de los datos
#define L_ENS160_AQI 0x21             // Registro de índice de calidad del aire
#define L_ENS160_TVOC 0x22            // Registro de compuestos orgánicos volátiles totales
#define L_ENS160_ECO2 0x24            // Registro de concentración de CO2
#define L_ENS160_ETOH 0x25            // Registro de concentración de etanol
#define L_ENS160_T 0x30               // Registro de temperatura (lectura)
#define L_ENS160_RH 0x32              // Registro de humedad relativa (lectura)
#define L_ENS160_DEVICE_STATUS 0x46   // Registro de estado del dispositivo
#define L_ENS160_GPR_READ 0x48        // Registro para lectura general
#define L_ENS160_GPR_WRITE 0x40       // Registro de escritura general

class L_ENS160
{
public:
  L_ENS160(TwoWire &wirePort = Wire);            // Constructor
  bool begin();                                  // Inicialización del sensor
  uint16_t readCO2();                            // Lectura de la concentración de CO2 equivalente
  uint8_t readAQI();                             // Lectura del índice de calidad del aire
  uint16_t readTVOC();                           // Lectura de compuestos orgánicos volátiles totales
  uint16_t readEthanol();                        // Lectura de concentración de etanol
  uint8_t getDeviceStatus();                     // Obtiene el estado del dispositivo
  uint8_t getDataStatus();                       // Obtiene el estado de los datos
  float readTemperature();                       // Lee la temperatura del sensor configurada
  float readHumidity();                          // Lee la humedad del sensor configurada
  void ambienteExterior(float temp, float hum);  // Configura temperatura y humedad exterior
  void setMode(uint8_t mode);                    // Configura el modo de operación
  void reset();                                  // Reinicia el sensor
  bool isDataReady();                            // Comorpbar si hay nuevos datos

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
