@ECHO OFF

IF NOT EXIST bin ( MKDIR bin)
start pwsh -NoExit -Command "g++ -s -O3 -std=c++20 -DNDEBUG -Iprog/include prog/source/*.cpp -lwinmm -o bin/TerminalMusicPlayer.exe"
