@echo off
setlocal
if "%~1"=="" (
    echo Usage: _zelda_release.bat "stock USA PRG0 ROM" ["locally patched HD ROM"]
    echo Derive the HD ROM with tools/apply_hd_patch.py before supplying it.
    exit /b 1
)
if "%~2"=="" (
    "%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -File "%~dp0tools\make_release.ps1" -Rom "%~f1"
) else (
    "%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -File "%~dp0tools\make_release.ps1" -Rom "%~f1" -HdRom "%~f2"
)
exit /b %errorlevel%
