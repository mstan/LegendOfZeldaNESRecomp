@echo off
setlocal
REM ===========================================================================
REM _zelda_release.bat - build BOTH Zelda variants in one shot.
REM
REM Produces, in build_release\ :
REM   LegendOfZeldaNESRecomp.exe      STOCK   - recompiled from the unpatched ROM,
REM                                            pure stock game, HD-pack UI hidden.
REM   LegendOfZeldaNESRecomp-HD.exe   HD      - recompiled from the patched "Zelda
REM                                            Remastered" ROM, HD-pack UI enabled.
REM
REM You must supply a legitimate stock PRG0 "Zelda # NES.NES" (SHA-1
REM dab79c84934f9aa5db4e7dad390e5d0c12443fa2) in the repo root. The patched ROM is
REM derived from it here (tools\apply_hd_patch.py + hdpatch\ZeldaHD.ips); neither
REM ROM is committed.
REM
REM Two builds exist because the recompiler is static: each exe bakes in ONE ROM's
REM code. The stock build is byte-identical stock Zelda; the HD build carries the
REM Remastered gameplay/text/audio and is what the HD texture pack is calibrated
REM against. See CLAUDE.md "Cutting a release".
REM ===========================================================================

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
set "PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja;%PATH%"
cd /d %~dp0

echo === [1/5] Build recompiler ===
cd nesrecomp
if not exist build_recomp\CMakeCache.txt (
    cmake -S recompiler -B build_recomp -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl
    if errorlevel 1 exit /b 1
)
cmake --build build_recomp
if errorlevel 1 exit /b 1
cd ..
set "RECOMP=nesrecomp\build_recomp\NESRecomp.exe"

echo === [2/5] Derive patched HD ROM ===
if not exist build mkdir build
python tools\apply_hd_patch.py --rom "Zelda # NES.NES" --ips hdpatch\ZeldaHD.ips --out build\zelda_hd.nes
if errorlevel 1 exit /b 2

REM Each variant regens the same game.toml under its OWN output prefix
REM (--output-prefix), so the recompiler writes generated\zelda_stock_* /
REM zelda_hd_* directly — umbrella, per-bank split parts, and dispatch all
REM carry the prefix, so the two variants' bank files never collide (the
REM per-bank split makes the old "regen then rename the umbrella" approach
REM unsafe, since both umbrellas would #include the same zelda_full_bankNN.c).

echo === [3/5] Regen STOCK (generated\zelda_stock_*) ===
"%RECOMP%" "Zelda # NES.NES" --game game.toml --output-prefix zelda_stock
if errorlevel 1 exit /b 3

echo === [4/5] Regen HD (generated\zelda_hd_*) ===
"%RECOMP%" build\zelda_hd.nes --game game.toml --output-prefix zelda_hd
if errorlevel 1 exit /b 4

echo === [5/5] Configure + build both targets ===
if not exist build_release\CMakeCache.txt (
    cmake -S . -B build_release -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
    if errorlevel 1 exit /b 5
)
cmake --build build_release
if errorlevel 1 exit /b 6

echo.
echo === DONE ===
echo   build_release\LegendOfZeldaNESRecomp.exe       (stock)
echo   build_release\LegendOfZeldaNESRecomp-HD.exe    (HD / Remastered)
