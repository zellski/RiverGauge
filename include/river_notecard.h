#pragma once

#include <river_reading.h>

bool notecard_setup(const char* productUID, const char* deviceNumber);
bool notecard_upload_reading(const RiverReading& reading);
void notecard_debug();
