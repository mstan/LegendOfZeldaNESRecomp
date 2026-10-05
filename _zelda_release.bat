@echo off
setlocal
if "%~1"=="" (
    echo Usage: _zelda_release.bat "stock USA PRG0 ROM"
    echo HD packs install through Mods in the same executable.
    exit /b 1
)
"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -File "%~dp0tools\make_release.ps1" -Rom "%~f1"
exit /b %errorlevel%
