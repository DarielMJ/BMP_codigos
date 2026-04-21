// In this file you can find the first BMP280 version , without temperature corrections:
#include "Adafruit_Sensor.h"
#include "Adafruit_INA219.h"
#include "Adafruit_BMP280.h"
#include <limits.h>

#define TIMEOUT_US 5000

Adafruit_BMP280 bmp;

/**
 * @brief Returns the current temperature multiplied by 100 to preserve 2 decimal places.
 */
unsigned short getTemperature(long* output) {

  unsigned long start = micros();

  // Raw data acquisition 
  float rawTemperature = bmp.readTemperature();

  // Timeout check
  if (micros() - start > TIMEOUT_US) {
    return 3;
  }

  // Data processing
  double tempScaled = rawTemperature * 100.0;

  // Error Validation 
  if (tempScaled > INT32_MAX) {  
    *output = INT32_MAX;
    return 1;  
  } 
  else if (tempScaled < -(INT32_MAX + 1)) {
    *output = -(INT32_MAX + 1);
    return 2;
  }

  *output = (long)tempScaled;
  return 0;
}

/**
 * @brief Returns the current presion (MPa) multiplied by 100 to preserve 2 decimal places.
 */
unsigned short getPressure(long* output) {

  unsigned long start = micros();

  // Raw data acquisition 
  float rawPressure = bmp.readPressure() / 1000.0; 

  // Timeout check
  if (micros() - start > TIMEOUT_US) {
    return 3;
  }

  // Data processing 
  double pressureScaled = rawPressure * 100.0;

  // Error Validation 
  if (pressureScaled > INT32_MAX) {
    *output = INT32_MAX;
    return 1;
  } 
  else if (pressureScaled < -(INT32_MAX + 1)) {
    *output = -(INT32_MAX + 1);
    return 2;
  }

  *output = (long)pressureScaled;
  return 0;
}

/**
 * @brief Returns the current altitude multiplied by 100 to preserve 2 decimal places.
 */
unsigned short getAltitude(long* output) {

  unsigned long start = micros();

  //Raw data acquisition 
  float rawAltitude = bmp.readAltitude(1013.25);

  // Timeout check
  if (micros() - start > TIMEOUT_US) {
    return 3;
  }

  // Data processing 
  double altitudeScaled = rawAltitude * 100.0;

  // Error Validation 
  if (altitudeScaled > INT32_MAX) {
    *output = INT32_MAX;
    return 1;
  } 
  else if (altitudeScaled < -(INT32_MAX + 1)) {
    *output = -(INT32_MAX + 1);
    return 2;
  }

  *output = (long)altitudeScaled;
  return 0;
}

void setup() {
  Serial.begin(9600);
  bmp.begin(0x76);

  // Continuous measurement mode
  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X1,
    Adafruit_BMP280::SAMPLING_X1,
    Adafruit_BMP280::FILTER_OFF,
    Adafruit_BMP280::STANDBY_MS_1
  );
}

void loop() {

  long temperature;
  long pressure;
  long altitude;

  unsigned short tStatus;
  unsigned short pStatus;
  unsigned short aStatus;

  tStatus = getTemperature(&temperature);
  pStatus = getPressure(&pressure);
  aStatus = getAltitude(&altitude);

  Serial.print("Temp: ");
  Serial.print(temperature / 100.0);
  Serial.print(" °C | Status: ");
  Serial.println(tStatus);

  Serial.print("Pressure: ");
  Serial.print(pressure / 100.0);
  Serial.print(" MPa | Status: ");
  Serial.println(pStatus);

  Serial.print("Altitude: ");
  Serial.print(altitude / 100.0);
  Serial.print(" m | Status: ");
  Serial.println(aStatus);

  delay(500);
}
