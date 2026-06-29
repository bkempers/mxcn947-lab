#include "common.h"

float celcius_to_farhenheit(struct sensor_value value)
{
    float fahrenheit = 0.0f;

    // val1 + val2 * 10^(-6)
    float temp_celcius = value.val1 + value.val2 * 1e-6f;

    fahrenheit = (temp_celcius * 1.8f) + 32;

    return fahrenheit;
}
