:floppy_disk: `bootloaders/UNO_R4`
====================================
Compiled with
```bash
git clone https://github.com/arduino/arduino-renesas-bootloader
git clone https://github.com/hathach/tinyusb
cd tinyusb
# This step is temporary
patch -p1 < ../arduino-renesas-bootloader/0001-fix-arduino-bootloaders.patch
python tools/get_deps.py ra
cd ..
cd arduino-renesas-bootloader
TINYUSB_ROOT=$PWD/../tinyusb make -f Makefile.minima 
# TINYUSB_ROOT=$PWD/../tinyusb make -f Makefile.wifi
```

:rescue_worker_helmet: `How to update ESP32-S3 firmware`
====================================

Prerequisite: install esp-idf v5.0.0 (or newer) and run
```
. $esp-idf-path/export.sh

cd extras/uno-r4-wifi-usb-bridge
```

Restart the board in ESP download mode by shorting ESP_DOWNLOAD with GND, then power via USB
The board will restart as Espressif USB JTAG/serial debug unit
Then run
```
idf.py -p $portname flash
```

:rocket: `How to load bootloader (UNO R4 WIFi)`
====================================

Once the bridge is up and running, it will respond to two peculiar baud rates:
* 1200bps: triggers the bootloader mode for RA4 chip
* 2400bps: triggers ROM DFU mode

So, to reflash the bootloader, change the port baud to 2400
```
stty -F $portname 2400
```

Then flash the bootloader with [`rfp-cli`](https://www.renesas.com/us/en/software-tool/renesas-flash-programmer-programming-gui#download)
```
rfp-cli -device ra -dtr -port $portname -p dfu_wifi.hex
```

:rocket: `How to load bootloader (UNO R4 Minima)`
====================================

Reboot the board with BOOT shorted to GND
The board will enumerate as Renesas RA USB Boot

Then flash the bootloader with [`rfp-cli`](https://www.renesas.com/us/en/software-tool/renesas-flash-programmer-programming-gui#download)
```
rfp-cli -device ra -port $portname -p dfu_minima.hex
```
