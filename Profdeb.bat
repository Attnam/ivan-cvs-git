@echo off
prep /om /ft main/debug/main.exe
if errorlevel 1 goto done
profile main/debug/main
if errorlevel 1 goto done
prep /m main/debug/main
if errorlevel 1 goto done
plist main/debug/main > main.lst
:done
