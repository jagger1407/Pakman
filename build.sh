#!/bin/bash
if [ ! -d "./build/" ]; then
    mkdir build
fi
gcc src/packer.c src/unpacker.c src/pakman.c src/pak.c src/utils.c -o build/pakman -O2