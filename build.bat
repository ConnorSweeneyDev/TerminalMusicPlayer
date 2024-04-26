@ECHO OFF

IF NOT EXIST bin ( MKDIR bin)
start pwsh -NoExit -Command "g++ prog/source/main.cpp -lwinmm -o bin/MusicPlayer.exe"
