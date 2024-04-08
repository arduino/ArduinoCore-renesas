:floppy_disk: `bootloaders/THINGPLUS_RA6M5`
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
TINYUSB_ROOT=$PWD/../tinyusb make -f Makefile.thingplus
```

:rocket: `How to load bootloader`
====================================

Pull the boot test point low and plug in the Thing Plus RA6M5.
The board will enumerate as Renesas RA USB Boot.

Then flash the bootloader with [`rfp-cli`](https://www.renesas.com/us/en/software-tool/renesas-flash-programmer-programming-gui#download)
```
rfp-cli -device ra -port COM# -p dfu.hex
```

Otherwise, the same hex file can be loaded through the SWD port (using a Jlink or E2 Emulator for example)
```
rfp-cli -device ra -tool e2 -if swd -p dfu.hex
```
