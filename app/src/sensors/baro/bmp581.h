#ifndef BMP581_H
#define BMP581_H

#include <stdbool.h>
#include <stdint.h>

static inline float celcius_to_farhenheit(float c) { return c * 9.0f / 5.0f + 32.0f; }

struct baro_data {
	float   pressure_hpa;
	float   altitude_m;
	float   temp_c;
	int64_t timestamp_ms;
};

int bmp581_init(void);
bool bmp581_get(struct baro_data *out);
void bmp581_set_sea_level_hpa(float hpa);

#endif // BMP581_H
