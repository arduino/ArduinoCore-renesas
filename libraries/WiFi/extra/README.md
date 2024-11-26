# Build script for esp-hosted (Portenta C33)

```
git clone https://github.com/espressif/esp-hosted.git --recursive
cd esp-hosted/
#git checkout release/fg-v0.0.5
git checkout fe0b10584417629908cee8141699c2f35ea05a25
patch -p1 < ../0001-Defaults-for-C33-BLE.patch
cd esp_hosted_fg/esp/esp_driver/
./esp-idf/install.sh esp32s3
. ./esp-idf/export.sh
cd network_adapter/
idf.py set-target esp32c3
cp ../../../../../sdkconfig.default.c33 sdkconfig
idf.py build
cd ../../../../../
python combine.py
```
# How to flash the firmware

```
espflash write-bin 0x0 ESP32-C3.bin
```

or

```
esptool.py --chip esp32c3 -p /dev/ttyACM0 -b 230400 --before=default_reset --after=hard_reset --no-stub write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB 0x0 ESP32-C3.bin
```
