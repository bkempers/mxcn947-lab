#ifndef COMMON_H
#define COMMON_H

#include <zephyr/drivers/sensor.h>

float celcius_to_farhenheit(struct sensor_value value);

#endif
