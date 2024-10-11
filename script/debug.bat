@ECHO OFF

SET COMMAND=pwsh -Command "gdb -tui ./binary/TerminalMusicPlayer.exe"
IF "%1" == "-wezterm" (wezterm cli spawn --cwd %CD% %COMMAND%)
IF "%1" == "" (%COMMAND%)
