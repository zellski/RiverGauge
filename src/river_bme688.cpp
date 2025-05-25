#include <river_bme688.h>
#include <river_reading.h>

#include <Arduino.h>

#include "Adafruit_BME680.h"
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

// for SPI if we use it later
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme(&Wire); // I2C

void bme688_setup()
{
    if (!bme.begin(0x77, true)) {
        Serial.println("Could not find a valid BME680 sensor, check wiring!");
        return;
    }
    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_16X);
    bme.setHumidityOversampling(BME680_OS_16X);
    bme.setPressureOversampling(BME680_OS_16X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
    Serial.println("BME680 sensor initialized successfully.");
}

bool bme688_fill_readings(RiverReading& reading)
{
    if (!bme.performReading()) {
        return false;
    }
    reading.airTemperature = bme.temperature;
    reading.humidity = bme.humidity;
    reading.pressure = bme.pressure / 100.0; // Convert to hPa
    reading.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    return true;
}
