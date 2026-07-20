#ifndef BMP581_H
#define BMP581_H

#include <stdbool.h>
#include <stdint.h>

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
