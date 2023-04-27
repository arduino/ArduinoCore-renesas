:floppy_disk: `bootloaders/SANTIAGO`
====================================
Sources for Uno R4 WIFi at git@github.com:bcmi-labs/tinyusb.git , commit 185ae31ff15a1013a5a890455220a29bad1ed209

Compiled with
```bash
cd examples/device/dfu && make BOARD=santiago_composta
```

Sources for Uno R4 Minima at git@github.com:bcmi-labs/tinyusb.git , commit b80fa8f4027c5aa4d9af5720a82e9238d8f792bb

Compiled with
```bash
cd examples/device/dfu && make BOARD=santiago
```

:rescue_worker_helmet: `How to update ESP32-S3 firmware`
====================================

Prerequisite: install esp-idf v5.0.0 (or newer) and run
```
. $esp-idf-path/export.sh

cd extras/santiago-leven-usb-bridge
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

Then flash the bootloader with
```
rfp-cli -device ra -port $portname -p dfu_leven.hex
```

:rocket: `How to load bootloader (UNO R4 Minima)`
====================================

Reboot the board with BOOT shorted to GND
The board will enumerate as Renesas RA USB Boot

Then flash the bootloader with
```
rfp-cli -device ra -port $portname -p dfu_minima.hex
```