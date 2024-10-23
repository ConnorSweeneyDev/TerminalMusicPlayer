@ECHO OFF

FOR /f %%a IN ('pwsh -NoProfile -Command "Get-CimInstance -ClassName Win32_Processor | Select-Object NumberOfLogicalProcessors"') DO SET CPU_COUNT_MULTIPLIER=%%a
SET FLAGS=-s -f make/main.mk -j%CPU_COUNT_MULTIPLIER% DEBUG=0
SET UTILITY=utility %FLAGS%
SET PREPARE=prepare %FLAGS%
SET BUILD=build %FLAGS%

SET COMMAND=pwsh -NoExit -Command "make %UTILITY% && make %PREPARE% && make %BUILD%"
IF "%1" == "-wezterm" (wezterm cli spawn --cwd %CD% %COMMAND%)
IF "%1" == "" (START %COMMAND%)
