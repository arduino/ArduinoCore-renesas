#!/bin/bash

if [ ! -f platform.txt ]; then
  echo Launch this script from the root core folder as ./extras/package.sh
  exit 2
fi 

VERSION=`cat platform.txt | grep "version=" | cut -f2 -d"="`
echo $VERSION

CORE_BASE=`basename $PWD`
cd ..
tar --exclude='*.vscode*' --exclude='*.git*' --exclude='*e2studio*' --exclude='*extras*' -cjhvf ArduinoCore-renesas-$VERSION.tar.bz2 $CORE_BASE
cd -
