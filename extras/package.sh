#!/bin/bash

if [ ! -f platform.txt ]; then
  echo Launch this script from the root core folder as ./extras/package.sh
  exit 2
fi 

VERSION=`cat platform.txt | grep "version=" | cut -f2 -d"="`
echo $VERSION

FILENAME=ArduinoCore-renesas-$VERSION.tar.bz2

CORE_BASE=`basename $PWD`
cd ..
tar --exclude='*.vscode*' --exclude='*.git*' --exclude='*e2studio*' --exclude='*extras*' -cjhvf $FILENAME $CORE_BASE
cd -

mv ../$FILENAME .

CHKSUM=`sha256sum $FILENAME | awk '{ print $1 }'`
SIZE=`wc -c $FILENAME | awk '{ print $1 }'`

cat extras/package_index.json.template |
# sed "s/%%BUILD_NUMBER%%/${BUILD_NUMBER}/" |
# sed "s/%%CURR_TIME%%/${CURR_TIME_SED}/" |
sed "s/%%VERSION%%/${VERSION}/" |
sed "s/%%FILENAME%%/${FILENAME}/" |
sed "s/%%CHECKSUM%%/${CHKSUM}/" |
sed "s/%%SIZE%%/${SIZE}/" > package_renesas_${VERSION}_index.json

cat package_renesas_${VERSION}_index.json
