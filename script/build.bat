@ECHO OFF

IF "%1" == "-wezterm" (
  SET TERM=wezterm cli spawn --cwd %CD% pwsh -NoExit -Command
) ELSE (
  SET TERM=pwsh -NoExit -Command
)

FOR /f %%a in ('powershell -Command "[int](python -c 'import multiprocessing as mp; print(int(mp.cpu_count() * 1.5))')"') do SET CPU_COUNT_MULTIPLIER=%%a
SET FLAGS=-s -f make/main.mk -j %CPU_COUNT_MULTIPLIER% DEBUG=0
SET UTILITY=utility %FLAGS%
SET PREPARE=prepare %FLAGS%
SET BUILD=build %FLAGS%

%TERM% "make %UTILITY% && make %PREPARE% && make %BUILD%"
