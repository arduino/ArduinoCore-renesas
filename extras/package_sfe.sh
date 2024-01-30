#!/bin/bash

if [ ! -f platform.txt ]; then
    echo Launch this script from the root core folder as ./extras/package_sfe.sh
    exit 2
fi

if [ ! -d ../ArduinoCore-API ]; then
    git checkout https://github.com/arduino/ArduinoCore-API.git ../ArduinoCore-API
fi

VERSION=`cat platform.txt | grep "version=" | cut -f2 -d"="`
echo $VERSION

# Thing Plus RA6M5

VARIANT=thingplus
EXCLUDE_TAGS=--exclude-tag-all=.unor4_only

FILENAME=ArduinoCore-renesas_$VARIANT-$VERSION.tar.bz2

git checkout platform.txt

sed -i 's/minima./#minima./g' boards.txt
sed -i 's/unor4wifi./#unor4wifi./g' boards.txt
sed -i 's/muxto./#muxto./g' boards.txt
sed -i 's/portenta_c33./#portenta_c33./g' boards.txt
sed -i 's/Arduino Renesas fsp Boards/SparkFun Renesas Thing Plus Boards/g' platform.txt

CORE_BASE=`basename $PWD`
cd ..
tar $EXCLUDE_TAGS --exclude='*.vscode*' --exclude='*.tar.*' --exclude='*.json*' --exclude '*.git*' --exclude='*e2studio*' --exclude='*extras*' -cjhvf $FILENAME $CORE_BASE
cd -

mv ../$FILENAME .

