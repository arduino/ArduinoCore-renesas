#!/bin/bash

CFILE=esp_hosted_config.pb-c.c
HFILE=esp_hosted_config.pb-c.h 

CFILE_DEST=../../
HFILE_DEST=../../

# generate the protobuf .c and .h files
protoc --c_out=. esp_hosted_config.proto


if test -f "$CFILE"; then
   mv "$CFILE" "$CFILE_DEST"
else 
   echo "[ERROR]: something went wrong $CFILE does not exist!"
fi

if test -f "$HFILE"; then
   mv "$HFILE" "$HFILE_DEST"
else 
   echo "[ERROR]: something went wrong $HFILE does not exist!"
fi

PROTOBUF_HEADER=../repo/protobuf-c/protobuf-c.h 
PROTOBUF_DEST_FOLDER=..

cp "$PROTOBUF_HEADER" "$PROTOBUF_DEST_FOLDER"
