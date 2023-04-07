#!/bin/sh

PORT=$1
ESPTOOL_PATH=$2

if [ x$PORT == x ]; then
echo Please specify the target serial port
exit 1
fi

if [ x$ESPTOOL_PATH == x ]; then
ESPTOOL_PATH=$PWD/esptool_py/esptool/
fi

PATH=$ESPTOOL_PATH:$PATH

esptool.py --chip esp32c3 -p $PORT -b 230400 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size 4MB 0x0 bootloader.bin 0x8000 partition-table.bin 0xd000 ota_data_initial.bin 0x10000 network_adapter.bin


