// Include the Arduino library for the Notecard
#include <Notecard.h>

Notecard notecard;

#include "Arduino.h"
#include "bme68xLibrary.h"

#ifndef PIN_CS
#define PIN_CS SS
#endif

Bme68x bme;

void setup()
{
    SPI.begin();

    delay(2500);
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    /* initializes the sensor based on SPI library */
    bme.begin(PIN_CS, SPI);

    if (bme.checkStatus()) {
        if (bme.checkStatus() == BME68X_ERROR) {
            Serial.println("Sensor error:" + bme.statusString());
            return;
        } else if (bme.checkStatus() == BME68X_WARNING) {
            Serial.println("Sensor Warning:" + bme.statusString());
        }
    }

    /* Set the default configuration for temperature, pressure and humidity */
    bme.setTPH();

    /* Set the heater configuration to 300 deg C for 100ms for Forced mode */
    bme.setHeaterProf(300, 100);

    Serial.println("TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%), Gas resistance(ohm), Status");

    // Initialize Notecard
    notecard.begin();
}

void loop()
{
    // test Notecard communicatio
    Serial.println("Testing Blues Notecard communication...");
    J* req = notecard.newRequest("card.random");
    JAddNumberToObject(req, "count", 100);
    J* rsp = notecard.requestAndResponse(req);
    if (rsp != NULL) {
        if (JGetObject(rsp, "err") == NULL) {
            Serial.print("Notecard andom number: ");
            Serial.println(JGetNumber(rsp, "count"));
        } else {
            Serial.print("Error: ");
            Serial.println(JGetString(rsp, "err"));
        }
    } else {
        Serial.println("No response received.");
    }
    Serial.println();

    Serial.println("Testing BME5688 communication...");

    bme68xData data;

    bme.setOpMode(BME68X_FORCED_MODE);
    delayMicroseconds(bme.getMeasDur());

    if (bme.fetchData()) {
        bme.getData(data);
        Serial.print(String(millis()) + ", ");
        Serial.print(String(data.temperature) + ", ");
        Serial.print(String(data.pressure) + ", ");
        Serial.print(String(data.humidity) + ", ");
        Serial.print(String(data.gas_resistance) + ", ");
        Serial.println(data.status, HEX);
    }

    Serial.println("–––––––––––––––––––––––––––––");
    Serial.println();

    delay(5000);
}
