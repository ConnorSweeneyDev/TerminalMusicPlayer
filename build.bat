@ECHO OFF

IF NOT EXIST bin ( MKDIR bin)
start pwsh -NoExit -Command "g++ prog/source/*.cpp -Iprog/include -lwinmm -o bin/MusicPlayer.exe"
