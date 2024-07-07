@ECHO OFF

start pwsh -NoExit -Command "make -j $(python3 -c 'import multiprocessing as mp; print(int(mp.cpu_count() * 1.5))')"
