@ECHO OFF

set FLAGS=-s -f "make/main.mk"

set CLEAN=clean %FLAGS%
make %CLEAN%
