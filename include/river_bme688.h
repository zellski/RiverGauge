#pragma once

#include <river_reading.h>

void bme688_setup();
bool bme688_fill_readings(RiverReading& reading);
void bme688_debug();
