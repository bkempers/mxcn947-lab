cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(mxcn947-lab)

set(SOURCES
    src/main.c
    src/common.c
    src/led/led.c
    src/sensors/baro/bmp581.c
    src/display/lvgl_main.c
)

target_sources(app PRIVATE ${SOURCES})
