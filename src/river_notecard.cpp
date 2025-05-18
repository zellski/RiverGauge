#include <Arduino.h>
#include <Notecard.h>
#include <river_notecard.h>
#include <river_reading.h>

Notecard notecard;

bool notecard_setup(const char* productUID, const char* deviceNumber)
{
    Serial.println("Initializing Notecard...");

    notecard.begin();

    J* req = NoteNewRequest("hub.set");
    JAddStringToObject(req, "product", productUID);
    JAddStringToObject(req, "sn", deviceNumber);
    JAddStringToObject(req, "mode", "continuous");
    JAddBoolToObject(req, "sync", true);
    if (!NoteRequest(req)) {
        Serial.println("Notecard 'hub.set' request failed.");
        return false;
    }

    req = NoteNewRequest("card.status");
    J* rsp = NoteRequestResponse(req);
    if (rsp != NULL) {
        Serial.print("Connected: ");
        Serial.println(JGetBool(rsp, "connected") ? "Yes" : "No");
        Serial.print("Status: ");
        Serial.println(JGetString(rsp, "status"));
        NoteDeleteResponse(rsp);
    }

    return true;
}

bool notecard_sync_and_wait(unsigned long timeout_ms = 30000)
{
    J* req = NoteNewRequest("hub.sync");
    if (!NoteRequest(req)) {
        Serial.println("Notecard 'hub.sync' request failed to start.");
        return false;
    }
    Serial.println("Sync initiated, waiting for completion...");

    unsigned long start_time = millis();
    bool sync_complete = false;

    while (!sync_complete && (millis() - start_time < timeout_ms)) {
        req = NoteNewRequest("hub.sync.status");
        J* rsp = NoteRequestResponse(req);

        if (rsp != NULL) {
            bool in_progress = JGetBool(rsp, "sync");

            const char* status = JGetString(rsp, "status");
            Serial.print("Sync status: ");
            Serial.print(status);

            // If not in progress, we've completed (success or error)
            if (!in_progress) {
                sync_complete = true;
                Serial.println(" - COMPLETE!");
            } else {
                Serial.println(" - still in progress");
            }

            NoteDeleteResponse(rsp);
        } else {
            Serial.println("Failed to get sync status");
        }

        // Only delay if we're still waiting
        if (!sync_complete) {
            delay(1000);
        }
    }

    // If we timed out
    if (!sync_complete) {
        Serial.println("Sync timed out after waiting");
        return false;
    }

    return true;
}

bool notecard_upload_reading(const RiverReading& reading)
{
    J* req = NoteNewRequest("note.add");
    JAddStringToObject(req, "file", "sensors.qo");
    J* body = JAddObjectToObject(req, "body");

    JAddNumberToObject(body, "water_level", reading.waterLevel);
    JAddNumberToObject(body, "airTemperature", reading.airTemperature);
    JAddNumberToObject(body, "humidity", reading.humidity);
    JAddNumberToObject(body, "pressure", reading.pressure);
    JAddNumberToObject(body, "altitude", reading.altitude);

    J* rsp = NoteRequestResponse(req);
    if (rsp != NULL) {
        if (JGetObject(rsp, "err") == NULL) {
            NoteDeleteResponse(rsp);

            if (!notecard_sync_and_wait(60000)) {
                // failed or timed out (errors will already have been printed)
                return false;
            }
            return true;
        }
        Serial.print("Error uploading to NoteHub: ");
        Serial.println(JGetString(rsp, "err"));
        NoteDeleteResponse(rsp);
        return false;
    }
    Serial.println("No response received from NoteHub.");
    return false;
}
