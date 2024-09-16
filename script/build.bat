@ECHO OFF

for /f %%a in ('powershell -Command "[int](python -c 'import multiprocessing as mp; print(int(mp.cpu_count() * 1.5))')"') do set CPU_COUNT_MULTIPLIER=%%a
set FLAGS=-s -f make/main.mk -j %CPU_COUNT_MULTIPLIER% DEBUG=0

set UTILITY=utility %FLAGS%
set PREPARE=prepare %FLAGS%
set BUILD=build %FLAGS%
start pwsh -NoExit -Command "make %UTILITY% && make %PREPARE% && make %BUILD%"
