:floppy_disk: `bootloaders/PORTENTA_C33`
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
TINYUSB_ROOT=$PWD/../tinyusb make -f Makefile.c33 
```

:rocket: `How to load bootloader`
====================================

Using any Portenta carrier **with USB A connector**, toggle the BOOT dip-switch, connect to the PC using a very uncommon USB-A to USB-A cable and reset the board. 
The board will enumerate as Renesas RA USB Boot

Then flash the bootloader with [`rfp-cli`](https://www.renesas.com/us/en/software-tool/renesas-flash-programmer-programming-gui#download)
```
rfp-cli -device ra -port $portname -p dfu_c33.hex
```

Otherwise, the same hex file can be loaded through the SWD port (using a Jlink for example)

