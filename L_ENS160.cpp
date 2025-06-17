#include "L_ENS160.h"

/**
 * @brief Constructor de la clase ENS160
 * @param wirePort Puerto I2C a utilizar
 */
L_ENS160::L_ENS160(TwoWire &wirePort)
{
  _i2cPort = &wirePort;          // Asigna el puerto I2C
  _address = L_ENS160_I2C_ADDRESS; // Asigna la dirección I2C del sensor
}

/**
 * @brief Inicializa el sensor ENS160
 * @return true si el sensor está funcionando correctamente
 */
bool L_ENS160::begin()
{
  _i2cPort->begin();
  
  // Reset forzado
  writeRegister(L_ENS160_OPMODE, L_ENS160_MODE_RESET);
  delay(250);
  
  // Configurar modo estándar con verificación
  writeRegister(L_ENS160_OPMODE, L_ENS160_MODE_STANDARD);
  delay(100);
  
  // Verificación alternativa
  uint8_t mode = readRegister(L_ENS160_OPMODE);
  if (mode != L_ENS160_MODE_STANDARD) {
    // Segundo intento
    writeRegister(L_ENS160_OPMODE, L_ENS160_MODE_STANDARD);
    delay(100);
    mode = readRegister(L_ENS160_OPMODE);
    if (mode != L_ENS160_MODE_STANDARD) return false;
  }
  
  return true;
}

/**
 * @brief Lee la concentración de CO2 del sensor
 * @return Concentración del CO2 en partes por millón (ppm)
 */
uint16_t L_ENS160::readCO2()
{
  uint8_t data[2];                     // Buffer para almacenar los datos leídos
  readRegisters(L_ENS160_ECO2, data, 2); // Supuesto registro del CO2
  dataUnion co2Union;                  // Unión de datos para interpretar los bytes
  co2Union.bytes[0] = data[0];         // Asignamos el byte menos significativo
  co2Union.bytes[1] = data[1];         // Asignamos el byte más significativo
  return co2Union.value_uint16;        // Retornamos el valor de la concentración de CO2
}

/**
 * @brief Lee el índice de calidad del aire (AQI)
 * @return Valor de AQI (1-5, donde 1 es excelente y 5 es pobre)
 */
uint8_t L_ENS160::readAQI() {
  return readRegister(L_ENS160_AQI);  // Lee directamente el registro AQI
}

/**
 * @brief Lee los compuestos orgánicos volátiles totales (TVOC)
 * @return Concentración de TVOC en partes por billón (ppb)
 */
uint16_t L_ENS160::readTVOC() {
  uint8_t data[2];                        // Buffer para almacenar los datos leídos
  readRegisters(L_ENS160_TVOC, data, 2);  // Lee el registro de TVOC
  dataUnion tvocUnion;                    // Unión de datos para interpretar los bytes
  tvocUnion.bytes[0] = data[0];           // Asignamos el byte menos significativo
  tvocUnion.bytes[1] = data[1];           // Asignamos el byte más significativo
  return tvocUnion.value_uint16;          // Retornamos el valor de TVOC
}

/**
 * @brief Lee la concentración de etanol
 * @return Concentración de etanol en partes por billón (ppb)
 */
uint16_t L_ENS160::readEthanol() {
  uint8_t data[2];                        // Buffer para almacenar los datos leídos
  readRegisters(L_ENS160_ETOH, data, 2);  // Lee el registro de etanol
  dataUnion ethanolUnion;                 // Unión de datos para interpretar los bytes
  ethanolUnion.bytes[0] = data[0];        // Asignamos el byte menos significativo
  ethanolUnion.bytes[1] = data[1];        // Asignamos el byte más significativo
  return ethanolUnion.value_uint16;       // Retornamos el valor de etanol
}

/**
 * @brief Obtiene el estado del dispositivo
 * @return Estado del dispositivo (bit 0: New data available, bit 1: New GSR available)
 */
uint8_t L_ENS160::getDeviceStatus() {
  // Solución definitiva para versiones que no reportan device status
  // Leemos el registro de operación como proxy del estado
  return readRegister(L_ENS160_OPMODE);
}

/**
 * @brief Obtiene el estado de los datos
 * @return Estado de los datos (bit 0: New data in DATA_AQI, bit 1: New data in DATA_TVOC, etc.)
 */
uint8_t L_ENS160::getDataStatus() {
  return readRegister(L_ENS160_STATUS);
}

bool L_ENS160::isDataReady() {
  // Interpretamos DATA_STATUS 0x8A como válido
  uint8_t status = readRegister(L_ENS160_STATUS);
  return (status & 0x80) && (status != 0xFF);
}

/**
 * @brief Lee la temperatura del sensor
 * @return Temperatura en grados Celsius
 */
float L_ENS160::readTemperature() {
  uint8_t data[2];                                  // Buffer para almacenar los datos leídos
  readRegisters(L_ENS160_T, data, 2);               // Lee el registro de temperatura
  dataUnion tempUnion;                              // Unión de datos para interpretar los bytes
  tempUnion.bytes[0] = data[0];                     // Asignamos el byte menos significativo
  tempUnion.bytes[1] = data[1];                     // Asignamos el byte más significativo
  return (tempUnion.value_uint16 / 64.0) - 273.15;  // Convertimos a Celsius
}

/**
 * @brief Lee la humedad del sensor
 * @return Humedad relativa en porcentaje
 */
float L_ENS160::readHumidity() {
  uint8_t data[2];                       // Buffer para almacenar los datos leídos
  readRegisters(L_ENS160_RH, data, 2);   // Lee el registro de humedad
  dataUnion humUnion;                    // Unión de datos para interpretar los bytes
  humUnion.bytes[0] = data[0];           // Asignamos el byte menos significativo
  humUnion.bytes[1] = data[1];           // Asignamos el byte más significativo
  return humUnion.value_uint16 / 512.0;  // Convertimos a porcentaje
}

/**
 * @brief Configura el modo de operación del sensor
 * @param mode Modo de operación (L_ENS160_MODE_DEEP_SLEEP, L_ENS160_MODE_IDLE, L_ENS160_MODE_STANDARD)
 */
void L_ENS160::setMode(uint8_t mode) {
  writeRegister(L_ENS160_OPMODE, mode);
}

/**
 * @brief Reinicia el sensor
 */
void L_ENS160::reset() {
  writeRegister(L_ENS160_OPMODE, L_ENS160_MODE_RESET);
  delay(10);  // Pequeña pausa para el reinicio
}

/**
 * @brief Envía la temperatura y humedad exterior al sensor para mejor precisión
 * @param temp Temperatura exterior en grados Celsius
 * @param hum Humedad exterior en porcentaje
 */
void L_ENS160::ambienteExterior(float temp, float hum)
{
  temperaturaExterior(temp); // Envía la temperatura exterior al sensor
  humedadExterior(hum);      // Envía la humeda exterior al sensor
}

/**
 * @brief Envía la temperatura exterior al sensor
 * @param temp Temperatura exterior en grados Celsius
 */
void L_ENS160::temperaturaExterior(float temp)
{
  dataUnion tempUnion;                                                  // Unión de datos para interpretar los bytes
  tempUnion.value_uint16 = static_cast<uint16_t>((temp + 273.15) * 64); // Convertimos a uint16_t0
  writeRegister(L_ENS160_TEMP_IN, tempUnion.bytes[0]);                    // Byte menos significativo
  writeRegister(L_ENS160_TEMP_IN + 1, tempUnion.bytes[1]);                // Byte más significativo
}

/**
 * @brief Envía la humedad exterior al sensor
 * @param hum Humedad exterior en porcentaje (%rH)
 */
void L_ENS160::humedadExterior(float hum)
{
  dataUnion humUnion;                                       // Unión de datos para interpretar los bytes
  humUnion.value_uint16 = static_cast<uint16_t>(hum * 512); // Convertimos a uint16_t
  writeRegister(L_ENS160_RH_IN, humUnion.bytes[0]);           // Byte menos significativo
  writeRegister(L_ENS160_RH_IN + 1, humUnion.bytes[1]);       // Byte más significativo
}

/**
 * @brief Lee un solo byte de un registro del sensor
 * @param reg Registro a leer
 * @return Valor del registro
 */
uint8_t L_ENS160::readRegister(uint8_t reg)
{
  _i2cPort->beginTransmission(_address);       // Inicia la transmisión al dispositivo
  _i2cPort->write(reg);                        // Escribe la dirección del registro
  _i2cPort->endTransmission(false);            // Finaliza la transmisión sin liberar el bus I2C
  _i2cPort->requestFrom(_address, (uint8_t)1); // Solicita un byte al sensor
  return _i2cPort->read();                     // Lee el byte recibido
}

/**
 * @brief Escribe un byte en un registro del sensor
 * @param reg Registro a escribir
 * @param value Valor a escribir
 */
void L_ENS160::writeRegister(uint8_t reg, uint8_t value)
{
  _i2cPort->beginTransmission(_address); // Inicia la transmisión al dispositivo
  _i2cPort->write(reg);                  // Escribe la dirección del registro
  _i2cPort->write(value);                // Envía el valor a escribir
  _i2cPort->endTransmission();           // Finaliza la transmisión
}

/**
 * @brief Lee varios registros del sensor
 * @param reg Registro a leer
 * @param buffer Buffer para almacenar los datos leídos
 * @param len Número de bytes a leer
 */
void L_ENS160::readRegisters(uint8_t reg, uint8_t *buffer, size_t len)
{
  _i2cPort->beginTransmission(_address); // Inicia la tranmisión al dispositivo
  _i2cPort->write(reg);                  // Escribe la dirección del primer registro a leer

  if (_i2cPort->endTransmission(false) != 0) // Finaliza la transmisión sin liberar el bus I2C
    return;                                  // Si hubo un error, salimos

  delay(10); // Ajusta este tiempo según sea necesario

  _i2cPort->requestFrom(_address, len); // Solicita los datos al sensor

  if ((size_t)_i2cPort->available() != len) // Verifica si se recibieron los datos esperados
    return;                         // Si no se recibieron todos los datos, salimos

  for (size_t i = 0; i < len; i++) // Lee los datos recibidos
    buffer[i] = _i2cPort->read();  // Almacena los datos en el buffer
}
