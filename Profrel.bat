@echo off
prep /om /ft main/release/main.exe
if errorlevel 1 goto done
profile main/release/main
if errorlevel 1 goto done
prep /m main/release/main
if errorlevel 1 goto done
plist main/release/main > main.lst
:done
