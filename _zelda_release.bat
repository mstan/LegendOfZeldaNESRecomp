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
cd /d F:\Projects\nesrecomp\LegendOfZeldaNESRecomp

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
python tools\apply_hd_patch.py --rom "Zelda # NES.NES" --ips hdpatch\ZeldaHD.ips --out build\zelda_hd.nes
if errorlevel 1 exit /b 2

REM The recompiler always writes generated\zelda_{full,dispatch}.c (output_prefix
REM in game.toml). We regen each ROM and rename to the per-variant prefix the
REM CMake targets expect (zelda_stock_* / zelda_hd_*).

echo === [3/5] Regen STOCK (generated\zelda_stock_*) ===
"%RECOMP%" "Zelda # NES.NES" --game game.toml
if errorlevel 1 exit /b 3
move /Y generated\zelda_full.c     generated\zelda_stock_full.c     >nul
move /Y generated\zelda_dispatch.c generated\zelda_stock_dispatch.c >nul

echo === [4/5] Regen HD (generated\zelda_hd_*) ===
"%RECOMP%" build\zelda_hd.nes --game game.toml
if errorlevel 1 exit /b 4
move /Y generated\zelda_full.c     generated\zelda_hd_full.c     >nul
move /Y generated\zelda_dispatch.c generated\zelda_hd_dispatch.c >nul

echo === [5/5] Configure + build both targets ===
if not exist build_release\CMakeCache.txt (
    cmake -S . -B build_release -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -DENABLE_NESTOPIA_ORACLE=OFF
    if errorlevel 1 exit /b 5
)
cmake --build build_release
if errorlevel 1 exit /b 6

echo.
echo === DONE ===
echo   build_release\LegendOfZeldaNESRecomp.exe       (stock)
echo   build_release\LegendOfZeldaNESRecomp-HD.exe    (HD / Remastered)
