#!/bin/sh -x

ARDUINO=arduino-cli
SKETCH_NAME="SDUBoot.ino"
SKETCH="$PWD/$SKETCH_NAME"
BUILD_PATH="$PWD/build"
OUTPUT_PATH="../../src/boot"

buildSDUBootSketch() {
	BOARD=$1
	DESTINATION=$2

	$ARDUINO compile -b $BOARD --output-dir="$BUILD_PATH" "$SKETCH"
	cat "$BUILD_PATH/$SKETCH_NAME.bin" | xxd -i > $DESTINATION
	rm -rf "$BUILD_PATH"
}

mkdir -p "$OUTPUT_PATH"

buildSDUBootSketch "my_boards:renesas_uno:unor4_sdu" "$OUTPUT_PATH/unor4.h"

