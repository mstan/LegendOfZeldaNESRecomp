@echo off
setlocal
git submodule update --init --recursive nesrecomp recomp-ui
if errorlevel 1 exit /b 1
echo Ready - pinned engine and UI initialized; cycle builds generate from your ROM.
