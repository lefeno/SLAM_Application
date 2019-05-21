/**
 * @file   Sensor.h
 * @brief  Sensor class - common header
 *
 * Copyright (C) 2015 Toshiba Corporation. All right reserved.
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "IesSdk.h"

IesSdk_ReturnValue_t Sensor_get_distance(int16_t* p_value);
#endif
