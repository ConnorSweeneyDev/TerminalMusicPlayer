@ECHO OFF

SET FLAGS=-s -f "make/main.mk"

SET CLEAN=clean %FLAGS%
make %CLEAN%
