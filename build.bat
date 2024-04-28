@ECHO OFF

IF NOT EXIST bin ( MKDIR bin)
start pwsh -NoExit -Command "g++ -g -Wall -Wextra -Wpedantic -Iprog/include prog/source/*.cpp -lwinmm -o bin/TerminalMusicPlayer.exe"
