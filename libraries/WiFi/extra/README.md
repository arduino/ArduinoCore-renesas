# Build script for esp-hosted (Portenta C33)

```
git clone https://github.com/espressif/esp-hosted.git --recursive
cd esp-hosted/
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
