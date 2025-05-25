
#include <Arduino.h>

#include <river_bme688.h>
#include <river_mb7040.h>
#include <river_notecard.h>

const char* PROJECT_ID = "com.alyx.par.winzell:river_gauge";
const char* SERIAL_NUMBER = "351077454485973";

void setup()
{
    delay(2500);
    Serial.begin(9600);
    while (!Serial) {
        delay(10);
    }

    notecard_setup(PROJECT_ID, SERIAL_NUMBER);

    //    bme688_setup();
    mb7040_setup();
}

void loop()
{
    Serial.println("River Gauge Main Loop");

    mb7040_debug();

    // RiverReading reading;
    // if (bme688_fill_readings(reading)) {
    //     Serial.print("Air Temperature: ");
    //     Serial.print(reading.airTemperature);
    //     Serial.println(" *C");

    //     Serial.print("Humidity: ");
    //     Serial.print(reading.humidity);
    //     Serial.println(" %");

    //     Serial.print("Pressure: ");
    //     Serial.print(reading.pressure);
    //     Serial.println(" hPa");
    // } else {
    //     Serial.println("Failed to read from BME688");
    // }

    // // TODO: add water level reading

    // if (notecard_upload_reading(reading)) {
    //     Serial.println("Reading uploaded successfully.");
    // } // else error will already have been printed
    // Serial.println("–––––––––––––––––––––––––––––");
    // Serial.println();

    // delay(1 * 60 * 1000); // 1 minute delay
    delay(1000);
}
