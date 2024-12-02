#!/bin/bash

PROCESSOR_COUNT=$(nproc)
DEBUG=0
FLAGS="-s -f make/main.mk -j$PROCESSOR_COUNT DEBUG=$DEBUG"

UTILITY="utility $FLAGS"
PREPARE="prepare $FLAGS"
BUILD="build $FLAGS"

if [ "$OS" == "Windows_NT" ]; then
  make $UTILITY && make $PREPARE && make $BUILD
else
  echo "Unsupported OS"
fi
