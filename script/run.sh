#!/bin/bash

if [ "$OS" == "Windows_NT" ]; then
  ./binary/TerminalMusicPlayer.exe
else
  echo "Unsupported OS"
fi
