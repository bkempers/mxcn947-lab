cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mxcn947-lab)

target_compile_definitions(app PRIVATE LV_LVGL_H_INCLUDE_SIMPLE=1)

set(DISPLAY ${CMAKE_CURRENT_SOURCE_DIR}/src/display)
file(GLOB_RECURSE DISPLAY_GEN
    ${DISPLAY}/display_gen.c
    ${DISPLAY}/components/*.c
    ${DISPLAY}/screens/*_gen.c
    ${DISPLAY}/widgets/*.c
    ${DISPLAY}/fonts/*.c)

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
    src/display/screens/motion.c
    ${DISPLAY_GEN}
)

target_sources(app PRIVATE ${SOURCES})
target_include_directories(app PRIVATE
    ${DISPLAY}
    ${DISPLAY}/components
    ${DISPLAY}/screens
    ${DISPLAY}/widgets)
