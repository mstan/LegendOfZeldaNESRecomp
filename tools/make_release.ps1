<#
make_release.ps1 -- build the Windows release zip for LegendOfZeldaNESRecomp.

Ships ONE windows zip (never a bare exe -- the exe needs SDL2.dll and the
launcher/ assets):

  LegendOfZeldaNESRecomp-windows-x64.zip
      LegendOfZeldaNESRecomp.exe + SDL2.dll + keybinds.ini + launcher/ + README.txt

The script builds build_release\ via _zelda_build.bat (recompiler + regen +
configure with oracle OFF + build), then stages and zips. The zip lands in
release\ (gitignored) and never contains debug.ini, config.ini, a ROM, or the
player's battery save (saves/).

Publish AFTER smoke-testing the zip from a scratch directory:

  gh release create vX.Y.Z release\LegendOfZeldaNESRecomp-windows-x64.zip `
      --title "vX.Y.Z -- <headline>" --notes-file RELEASE_NOTES.md

Usage: powershell -File tools\make_release.ps1 [-SkipBuild]
#>
param(
  [switch]$SkipBuild
)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$bin  = Join-Path $root 'build_release'
$out  = Join-Path $root 'release'
New-Item -ItemType Directory -Force $out | Out-Null

if (-not $SkipBuild) {
  # Use the explicit Windows command processor ($env:ComSpec). A bare `cmd`
  # can resolve to an msys2/devkitPro shim earlier on PATH, which silently
  # no-ops the .bat (exit 0, nothing built) instead of running it.
  & "$env:ComSpec" /c (Join-Path $root '_zelda_build.bat')
  if ($LASTEXITCODE -ne 0) { throw "_zelda_build.bat failed ($LASTEXITCODE)" }
}

$exe = Join-Path $bin 'LegendOfZeldaNESRecomp.exe'
if (-not (Test-Path $exe)) { throw "missing $exe -- run _zelda_build.bat first" }

$readme = @'
The Legend of Zelda - Static Recompilation
==========================================

A native PC build of The Legend of Zelda, produced by statically recompiling
the NES ROM's 6502 code to C with the NESRecomp framework
(github.com/mstan/nesrecomp).

No ROM is included. On first launch, select your legally-obtained Legend of
Zelda (USA) ROM. The path is remembered for future launches.

SAVES
-----
The Legend of Zelda saves to battery-backed SRAM. This build writes that save
to saves\ next to the exe (one .srm per registered file), exactly as the
cartridge battery would, so your three save slots persist across launches. The
pre-boot launcher's SAVE panel manages it (import / clear).

Controls: arrow keys = D-Pad, Z = A, X = B, Enter = Start, Tab = Select.
F5 turbo, F6 save state, F7 load state. Gamepads are supported; all bindings
are configurable in keybinds.ini.
'@

$stage = Join-Path $out 'stage'
if (Test-Path $stage) { Remove-Item -Recurse -Force $stage }
New-Item -ItemType Directory -Force $stage | Out-Null

Copy-Item $exe $stage
foreach ($extra in 'SDL2.dll', 'keybinds.ini') {
  $p = Join-Path $bin $extra
  if (Test-Path $p) { Copy-Item $p $stage }
}
$launcher = Join-Path $bin 'launcher'
if (Test-Path $launcher) { Copy-Item -Recurse $launcher (Join-Path $stage 'launcher') }

$readme | Out-File -Encoding ascii (Join-Path $stage 'README.txt')

# Belt-and-braces: never ship debug/dev artifacts, a ROM, or the player's save.
foreach ($banned in 'debug.ini', 'config.ini', 'baserom.nes', 'rom.cfg', 'dispatch_misses.log') {
  $p = Join-Path $stage $banned
  if (Test-Path $p) { Remove-Item $p }
}
$savesDir = Join-Path $stage 'saves'
if (Test-Path $savesDir) { Remove-Item -Recurse -Force $savesDir }

$zip = Join-Path $out 'LegendOfZeldaNESRecomp-windows-x64.zip'
if (Test-Path $zip) { Remove-Item $zip }
Compress-Archive -Path (Join-Path $stage '*') -DestinationPath $zip
Remove-Item -Recurse -Force $stage
Write-Host "staged $zip"
