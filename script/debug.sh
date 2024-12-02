#!/bin/bash

if [ "$OS" == "Windows_NT" ]; then
  gdb -tui ./binary/TerminalMusicPlayer.exe
else
  echo "Unsupported OS"
fi
