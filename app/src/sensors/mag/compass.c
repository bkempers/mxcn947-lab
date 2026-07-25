#include <math.h>
#include "compass.h"

/* Compass heading in degrees [0,360). North = 0, increasing clockwise.
 *   mag[]   : magnetometer x,y,z (gauss)
 *   accel[] : accelerometer x,y,z (m/s^2), for tilt compensation
 *   cal     : hard-iron/scale correction, or NULL to skip
 *   decl    : magnetic declination for your location (+east), 0 for magnetic north
 */
float compass_heading(const float mag[3], const float accel[3], float decl_deg)
{
    float roll  = atan2f(accel[1], accel[2]);
    float pitch = atan2f(-accel[0], sqrtf(accel[1]*accel[1] + accel[2]*accel[2]));
    float sr = sinf(roll),  cr = cosf(roll);
    float sp = sinf(pitch), cp = cosf(pitch);

    float xh = mag[0]*cp + mag[2]*sp;
    float yh = mag[0]*sr*sp + mag[1]*cr - mag[2]*sr*cp;

    float h = atan2f(-yh, xh) * (180.0f / (float)3.1415) + decl_deg;
    if (h < 0.0f)    h += 360.0f;
    if (h >= 360.0f) h -= 360.0f;
    return h;
}
