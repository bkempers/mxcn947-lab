cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mxcn947-lab)

set(SOURCES
    src/main.c
    src/common.c
    src/led/led.c
    src/sensors/baro/bmp581.c
    src/sensors/imu/lsm6dsox.c
    src/sensors/mag/lis3mdl_m.c
    src/sensors/mag/compass.c
    src/sensors/mag/mag_calibration.c
    src/display/lvgl_main.c
    src/display/theme/lvgl_theme.c
    src/display/widgets/lvgl_widgets.c
    src/display/screens/home.c
    src/display/screens/sensors.c
)

target_sources(app PRIVATE ${SOURCES})
