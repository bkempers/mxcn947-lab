# frdm-mxcn947 NXP Development Board Zephyr Development

Target: `frdm_mcxn947/mcxn947/cpu0`

Final hardware set:

| Bus | Device | Address / config | Zephyr compatible |
|---|---|---|---|
| LPI2C | BMP581 | 0x46 or 0x47 | `bosch,bmp581` |
| LPI2C | SHT4x | 0x44 | `sensirion,sht4x` |
| LPI2C | LSM6DSO-family IMU | 0x6A | `st,lsm6dso` |
| LPI2C | LIS3MDL | 0x1C | `st,lis3mdl-magn` |
| LPI2C | VEML7700 *(optional)* | 0x10 | `vishay,veml7700` |
| LPUART | Adafruit Ultimate GPS | 9600 8N1 | `gnss-nmea-generic` |
| LPSPI + 3 GPIO | ESP32-C6 | esp-hosted | `espressif,esp-hosted` |
| FlexIO | ST7789V | already working | (existing) |


---


