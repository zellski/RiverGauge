#pragma once

struct RiverReading {
    float waterLevel = -1.0; // in meters
    float airTemperature = -1.0; // in degrees Celsius
    float humidity = -1.0; // in percentage
    float pressure = -1.0; // in hPa
    float altitude = -1.0; // in meters
    // uint32_t timestamp; // Unix timestamp
};
