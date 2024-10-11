@ECHO OFF

SET COMMAND=binary/TerminalMusicPlayer.exe
IF "%1" == "-wezterm" (wezterm cli spawn --cwd %CD% pwsh -Command "./%COMMAND%")
IF "%1" == "" (cmd /C "start %COMMAND%")
