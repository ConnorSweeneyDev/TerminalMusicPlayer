@ECHO OFF

FOR /f %%a in ('powershell -Command "[int](python -c 'import multiprocessing as mp; print(int(mp.cpu_count() * 1.5))')"') do SET CPU_COUNT_MULTIPLIER=%%a
SET FLAGS=-s -f make/main.mk -j %CPU_COUNT_MULTIPLIER% DEBUG=0

SET UTILITY=utility %FLAGS%
SET PREPARE=prepare %FLAGS%
SET BUILD=build %FLAGS%
START pwsh -NoExit -Command "make %UTILITY% && make %PREPARE% && make %BUILD%"
