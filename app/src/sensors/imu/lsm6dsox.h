#ifndef LSM6DSOX_H
#define LSM6DSOX_H

#include <stdbool.h>
#include <stdint.h>

struct imu_data {
    float accel[3];
    float gyro[3];
    int64_t timestamp_ms;
};

int lsm6dsox_init(void);
bool lsm6dsox_get(struct imu_data *out);

#endif // LSM6DSOX_H
