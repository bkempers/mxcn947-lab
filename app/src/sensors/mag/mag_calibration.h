#ifndef MAG_CALIBRATION_H
#define MAG_CALIBRATION_H

struct mag_cal {
    float off[3];    /* hard-iron bias to subtract (gauss) */
    float scale[3];  /* per-axis scale; 1.0f = no soft-iron correction */
};

int  mag_cal_init(void);
void mag_cal_apply(float mag[3]);
void mag_cal_start(void);
int  mag_cal_finish(struct mag_cal *out);

#endif // MAG_CALIBRATION_H
