@ECHO OFF

IF "%1" == "-wezterm" (
  SET TERM=wezterm cli spawn --cwd %CD% pwsh -Command
) ELSE (
  SET TERM=pwsh -Command
)

%TERM% "./binary/TerminalMusicPlayer.exe"
