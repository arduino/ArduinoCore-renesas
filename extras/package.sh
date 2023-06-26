#!/bin/bash

if [ ! -f platform.txt ]; then
  echo Launch this script from the root core folder as ./extras/package.sh
  exit 2
fi

if [ ! -d ../ArduinoCore-API ]; then
  git clone git@github.com:arduino/ArduinoCore-API.git ../ArduinoCore-API
fi

VERSION=`cat platform.txt | grep "version=" | cut -f2 -d"="`
echo $VERSION

#portenta

VARIANT=portenta
EXCLUDE_TAGS=--exclude-tag-all=.unor4_only

FILENAME=ArduinoCore-renesas_$VARIANT-$VERSION.tar.bz2

git checkout boards.txt
git checkout platform.txt

sed -i 's/minima./#minima./g' boards.txt
sed -i 's/unor4wifi./#unor4wifi./g' boards.txt
sed -i 's/muxto./#muxto./g' boards.txt
sed -i 's/Arduino Renesas fsp Boards/Arduino Renesas Portenta Boards/g' platform.txt

CORE_BASE=`basename $PWD`
cd ..
tar $EXCLUDE_TAGS --exclude='*.vscode*' --exclude='*.tar.*' --exclude='*.json*' --exclude='*.git*' --exclude='*e2studio*' --exclude='*extras*' -cjhvf $FILENAME $CORE_BASE
cd -

mv ../$FILENAME .

CHKSUM=`sha256sum $FILENAME | awk '{ print $1 }'`
SIZE=`wc -c $FILENAME | awk '{ print $1 }'`

cat extras/package_index.json.template |
# sed "s/%%BUILD_NUMBER%%/${BUILD_NUMBER}/" |
# sed "s/%%CURR_TIME%%/${CURR_TIME_SED}/" |
sed "s/%%VERSION%%/${VERSION}/" |
sed "s/%%FILENAME_PORTENTA%%/${FILENAME}/" |
sed "s/%%CHECKSUM_PORTENTA%%/${CHKSUM}/" |
sed "s/%%SIZE_PORTENTA%%/${SIZE}/" > package_renesas_${VERSION}_index.json.tmp

#uno r4

VARIANT=uno
EXCLUDE_TAGS=--exclude-tag-all=.portenta_only

FILENAME=ArduinoCore-renesas_$VARIANT-$VERSION.tar.bz2

git checkout boards.txt
git checkout platform.txt

sed -i 's/portenta_c33./#portenta_c33./g' boards.txt
sed -i 's/muxto./#muxto./g' boards.txt
sed -i 's/Arduino Renesas fsp Boards/Arduino Renesas UNO R4 Boards/g' platform.txt

CORE_BASE=`basename $PWD`
cd ..
tar $EXCLUDE_TAGS --exclude='*.vscode*' --exclude='*.tar.*' --exclude='*.json*' --exclude='*.git*' --exclude='*e2studio*' --exclude='*extras*' -cjhvf $FILENAME $CORE_BASE
cd -

mv ../$FILENAME .

CHKSUM=`sha256sum $FILENAME | awk '{ print $1 }'`
SIZE=`wc -c $FILENAME | awk '{ print $1 }'`

cat package_renesas_${VERSION}_index.json.tmp |
# sed "s/%%BUILD_NUMBER%%/${BUILD_NUMBER}/" |
# sed "s/%%CURR_TIME%%/${CURR_TIME_SED}/" |
sed "s/%%VERSION%%/${VERSION}/" |
sed "s/%%FILENAME_UNO%%/${FILENAME}/" |
sed "s/%%CHECKSUM_UNO%%/${CHKSUM}/" |
sed "s/%%SIZE_UNO%%/${SIZE}/" > package_renesas_${VERSION}_index.json

cat package_renesas_${VERSION}_index.json

git checkout boards.txt
git checkout platform.txt