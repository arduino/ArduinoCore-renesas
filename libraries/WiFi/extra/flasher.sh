#!/bin/bash

PORT=$1
ESPTOOL_PATH=$2

if [ x$PORT == x ]; then
echo Please specify the target serial port
exit 1
fi

if [ x$ESPTOOL_PATH == x ]; then
ESPTOOL_PATH=$PWD/esptool/
fi

PATH=$ESPTOOL_PATH:$PATH

if [ -f "$ESPTOOL_PATH/esptool.py" ]; then
ESPTOOL_CMD=esptool.py
else
ESPTOOL_CMD=esptool
fi

$ESPTOOL_CMD --chip esp32c3 -p $PORT -b 230400 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB 0x0 bootloader.bin 0x8000 partition-table.bin 0x10000 network_adapter.bin


