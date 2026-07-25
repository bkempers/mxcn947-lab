#ifndef LIS3MDL_M_H
#define LIS3MDL_M_H

#include <stdbool.h>
#include <stdint.h>
#include "mag_calibration.h"

struct mag_data {
    float mag[3];
    float heading;
    float temp;
    int64_t timestamp_ms;
};

int lis3mdl_m_init(void);
bool lis3mdl_m_get(struct mag_data *out);

#endif // LIS3MDL_M_H
