#!/bin/bash

FLAGS="-s -f make/main.mk"

CLEAN="clean $FLAGS"

if [ "$OS" == "Windows_NT" ]; then
  make $CLEAN
else
  echo "Unsupported OS"
fi
