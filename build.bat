@ECHO OFF

IF NOT EXIST binary ( MKDIR binary)
start pwsh -NoExit -Command "g++ -s -O3 -std=c++20 -DNDEBUG -Iprogram/include program/source/*.cpp -lwinmm -o binary/TerminalMusicPlayer.exe"
