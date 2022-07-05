#!/bin/bash

CORE_PATH=$(pwd)
TARGET=UNO_R4

LIBRARY=`find . | grep "\.a$"`

echo Copying ${LIBRARY} to ${CORE_PATH}/../../${TARGET}/libs/libfsp.a
if [ ! -d ${CORE_PATH}/../../${TARGET}/libs ]
then
    mkdir ${CORE_PATH}/../../${TARGET}/libs
fi
cp ${LIBRARY} ${CORE_PATH}/../../${TARGET}/libs/libfsp.a

LINKER_SCRIPTS=`find . | grep "\.ld$"`
cp ${LINKER_SCRIPTS} ${CORE_PATH}/../../${TARGET}/

FILE_MK=`find . | grep subdir.mk | head -n1`

CCCOMMAND=`cat $FILE_MK | grep \$\(file | cut -f2 -d","`

echo $CCCOMMAND

DEFINES=()
INCLUDES=()
FLAGS=()

OIFS=$IFS
IFS=' '
tokens=$CCCOMMAND
for x in $tokens
do
    if [[ $x == -D* ]]; then DEFINES+=( $x ); fi
    if [[ $x == -I* ]]; then INCLUDES+=( $x ); fi
    if [[ $x == -m* ]]; then FLAGS+=( $x ); fi
    if [[ $x == -f* ]]; then FLAGS+=( $x ); fi
    if [[ $x == -W* ]]; then FLAGS+=( $x ); fi
done
IFS=$OIFS

# Clean files
rm ${CORE_PATH}/../../${TARGET}/defines.txt
rm ${CORE_PATH}/../../${TARGET}/includes.txt
rm ${CORE_PATH}/../../${TARGET}/cflags.txt
rm ${CORE_PATH}/../../${TARGET}/cxxflags.txt

for value in "${DEFINES[@]}"
do
    echo $value >> ${CORE_PATH}/../../${TARGET}/defines.txt
done

echo ${INCLUDES[@]}

if [ ! -d ${CORE_PATH}/../../${TARGET}/includes/ ]
then
    mkdir ${CORE_PATH}/../../${TARGET}/includes/
else
    rm -r ${CORE_PATH}/../../${TARGET}/includes/*
fi

for value in "${INCLUDES[@]}"
do
    INCLUDE_PATH=`echo $value | cut -f2 -d"\"" | cut -f1 -d"\""`
    echo $INCLUDE_PATH
    # temporarily, copy everything staring with "ra_" in variant/includes/ , everything with ra in core folder
    if [[ $INCLUDE_PATH == $PWD/ra* ]]; then
        INCLUDE_PATH_REL=${INCLUDE_PATH#"$PWD/"}
        cp -r --parent $INCLUDE_PATH_REL ${CORE_PATH}/../../${TARGET}/includes/
        echo "-iwithprefixbefore/variants/${TARGET}/includes/$INCLUDE_PATH_REL" >> ${CORE_PATH}/../../${TARGET}/includes.txt
    fi
    #rel_path=`echo $value | sed -e "s#-I$PWD#-iwithprefixbefore/fsp#g"`
    #echo $rel_path >> ${CORE_PATH}/variants/${TARGET}/includes.txt

    # TODO: check how many include folders are generated and if it makes sense to track them manually
done

rm -r ${CORE_PATH}/../../${TARGET}/includes/ra/*

RA_INCLUDES=`find ra/ -iname *.h`
cp --parent $RA_INCLUDES ${CORE_PATH}/../../${TARGET}/includes/

for value in "${FLAGS[@]}"
do
    echo $value >> ${CORE_PATH}/../../${TARGET}/cflags.txt
    echo $value >> ${CORE_PATH}/../../${TARGET}/cxxflags.txt
done