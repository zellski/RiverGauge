#include <river_mb7040.h>
#include <river_reading.h>

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_I2CDevice.h"

Adafruit_I2CDevice* mb7040_i2c_dev = NULL;

const uint8_t RangeCommand = 0x51; // Command to read range from MB7040

void mb7040_setup()
{
    mb7040_i2c_dev = new Adafruit_I2CDevice(0x70, &Wire);
    if (!mb7040_i2c_dev->begin()) {
        Serial.println("Could not find a valid MB7040 sensor, check wiring!");
        return;
    }
    if (!mb7040_i2c_dev->detected()) {
        Serial.println("MB7040 sensor not detected!");
        return;
    }
    Serial.println("MB7040 sensor initialized successfully.");
}

void mb7040_debug()
{
    if (!mb7040_i2c_dev) {
        Serial.println("MB7040 not initialized");
        return;
    }
    if (!mb7040_i2c_dev->write(&RangeCommand, 1)) {
        Serial.println("Failed to write command to MB7040");
        return;
    }
    delay(500); // Wait for the sensor to process the command

    uint8_t rangeBuf[4] = { 0, 0, 0, 0 };
    if (!mb7040_i2c_dev->read(rangeBuf, 4)) {
        Serial.println("Failed to read from MB7040");
        return;
    }
    Serial.print("BYTES: ");
    for (int i = 0; i < 4; i++) {
        Serial.print(rangeBuf[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    const int range = ((rangeBuf[0] << 8) | rangeBuf[1]);
    Serial.print("Water Level: ");
    Serial.println(range);
}

bool mb7040_fill_readings(RiverReading& reading)
{
    uint8_t rangeBuf[2] = { 0, 0 };
    if (!mb7040_i2c_dev->write_then_read(&RangeCommand, 1, rangeBuf, 2)) {
        Serial.println("Failed to read from MB7040");
        return false;
    }

    reading.waterLevel = ((rangeBuf[0] << 8) | rangeBuf[1]) / 100.0f; // Convert to meters
    Serial.print("Water Level: ");
    Serial.print(reading.waterLevel);
    return true;
}
