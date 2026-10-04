<#
make_release.ps1 -- build the Windows release zip for LegendOfZeldaNESRecomp.

Ships ONE windows zip (never a bare exe -- the exe needs SDL2.dll and the
launcher assets):

  LegendOfZeldaNESRecomp-windows-x64.zip
      LegendOfZeldaNESRecomp.exe + SDL2.dll + README.txt
      + assets/ (launcher fonts/images) + mods/ (default-off preloaded packages)

The script builds build_release\ via _zelda_release.bat (recompiler + regen +
configure with production observability OFF + build), then stages, verifies
and zips. The zip lands in release\ (gitignored). Staging is an allowlist: a
ROM, debug.ini, config.ini, saves/, mods/state.toml or any other dev payload
fails the build instead of shipping.

Smoke the zip before publishing:

  python tools\voxel_smoke.py --rom "Zelda # NES.NES"
  gh release create vX.Y.Z release\LegendOfZeldaNESRecomp-windows-x64.zip `
      --title "vX.Y.Z -- <headline>" --notes-file <notes.md>

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
  & "$env:ComSpec" /c (Join-Path $root '_zelda_release.bat')
  if ($LASTEXITCODE -ne 0) { throw "_zelda_release.bat failed ($LASTEXITCODE)" }
}

$exe = Join-Path $bin 'LegendOfZeldaNESRecomp.exe'
if (-not (Test-Path $exe)) { throw "missing $exe -- run _zelda_release.bat first" }
$cmakeCache = Join-Path $bin 'CMakeCache.txt'
if (-not (Test-Path $cmakeCache)) { throw "missing $cmakeCache -- run _zelda_release.bat first" }
if (-not (Select-String -LiteralPath $cmakeCache -Pattern '^NESRECOMP_ENABLE_TRACE:BOOL=OFF$')) {
  throw 'refusing to package a build with NESRECOMP_ENABLE_TRACE enabled or unset'
}

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

Controls: arrow keys = D-Pad, Z = A, X = B, Enter = Start,
Backslash = Select. Hold Tab for turbo. F1-F12 load save slots;
Shift+F1-F12 save those slots. Alt+Enter toggles fullscreen.
Gamepads are supported; bindings are configurable in the launcher's
Controls page or in keybinds.ini (created next to the exe on first launch).

VOXEL 3D (EXPERIMENTAL)
-----------------------
Open Mods in the launcher and enable Voxel 3D Overworld (Experimental) or
Voxel 3D First Person (Experimental). Both are disabled by default, mutually
exclusive, and change presentation only: the stock ROM and save data are
untouched. Numpad 0 toggles the view; numpad 8/2/4/6/7/9/+/-/1/3 adjust the
live camera and Numpad 5 resets it. Title, registration and inventory screens
stay flat. Expect visual rough edges; this mode is experimental.
'@

# No keybinds.ini: the runtime writes current defaults on first launch, so a
# stale build-tree copy (old Select/turbo keys) can never ship.
$required = @('LegendOfZeldaNESRecomp.exe', 'SDL2.dll', 'README.txt')
$requiredAssets = @(
  'assets/fonts/LatoLatin-Bold.ttf',
  'assets/fonts/LatoLatin-Regular.ttf',
  'assets/fonts/NotoSansSymbols2-Regular.ttf',
  'assets/fonts/OpenMoji-black-glyf.ttf',
  'assets/img/boxart.tga',
  'assets/img/brand_mark.tga',
  'assets/img/brand_nes.tga',
  'assets/img/flags.png',
  'assets/img/pad_nes.tga',
  'assets/img/verdict_bad.tga',
  'assets/img/verdict_none.tga',
  'assets/img/verdict_ok.tga',
  'assets/img/verdict_warn.tga'
)

function Get-StageRelativePath([string]$base, [string]$path) {
  return $path.Substring($base.Length).TrimStart('\', '/').Replace('\', '/')
}

function Assert-ReleaseStage([string]$stage, [string]$sourceMods) {
  $files = @(Get-ChildItem $stage -Recurse -File)
  $relativeFiles = @($files | ForEach-Object { Get-StageRelativePath $stage $_.FullName })

  $missing = @($required | Where-Object { $_ -notin $relativeFiles })
  if ($missing.Count -ne 0) {
    throw "release staging is missing required payload: $($missing -join ', ')"
  }
  $stagedAssets = @($relativeFiles | Where-Object { $_ -like 'assets/*' })
  if (@(Compare-Object $requiredAssets $stagedAssets).Count -ne 0) {
    throw 'release staging launcher asset inventory differs from the approved NES launcher assets'
  }

  # Allowlist: anything not named above or a preloaded manifest is rejected.
  $unexpected = @($relativeFiles | Where-Object {
    if ($_ -in $required) { return $false }
    if ($_ -in $requiredAssets) { return $false }
    if ($_ -match '^mods/packages/[^/]+/[^/]+/manifest\.toml$') { return $false }
    return $true
  })
  if ($unexpected.Count -ne 0) {
    throw "release staging contains forbidden owner/dev payload: $($unexpected -join ', ')"
  }

  # The staged catalog must mirror source-controlled mods/preloaded byte for byte.
  $sourceRoot = (Resolve-Path $sourceMods).Path
  $sourceManifests = @(Get-ChildItem $sourceRoot -Recurse -File)
  if ($sourceManifests.Count -eq 0) { throw 'source preloaded mod catalog is empty' }
  $expectedModPaths = @()
  foreach ($source in $sourceManifests) {
    $relative = Get-StageRelativePath $sourceRoot $source.FullName
    if ($relative -notmatch '^packages/[^/]+/[^/]+/manifest\.toml$') {
      throw "source preloaded catalog contains non-manifest payload: $relative"
    }
    $stagedRelative = "mods/$relative"
    $expectedModPaths += $stagedRelative
    $stagedPath = Join-Path $stage $stagedRelative.Replace('/', '\')
    if (-not (Test-Path -LiteralPath $stagedPath -PathType Leaf)) {
      throw "release staging is missing preloaded manifest: $stagedRelative"
    }
    if ((Get-FileHash -Algorithm SHA256 -LiteralPath $source.FullName).Hash -ne
        (Get-FileHash -Algorithm SHA256 -LiteralPath $stagedPath).Hash) {
      throw "release staging modified preloaded manifest: $stagedRelative"
    }
  }
  foreach ($voxel in 'voxel-diorama', 'voxel-first-person') {
    if (-not ($expectedModPaths -match "legend-of-zelda\.enhancement\.$voxel/")) {
      throw "release catalog is missing the $voxel package"
    }
  }
  $stagedModPaths = @($relativeFiles | Where-Object { $_ -like 'mods/*' })
  if (@(Compare-Object $expectedModPaths $stagedModPaths).Count -ne 0) {
    throw 'release staging mod inventory differs from pristine mods/preloaded'
  }

  # Reject machine-local absolute paths in every text payload.
  foreach ($file in $files | Where-Object { $_.Extension -in '.txt', '.ini', '.toml' }) {
    $text = Get-Content -Raw -LiteralPath $file.FullName
    if ($text -match '(?im)(?:[a-z]:[\\/]|/(?:home|users|tmp)/)') {
      throw "release text contains a machine-local absolute path: $(Get-StageRelativePath $stage $file.FullName)"
    }
  }
}

function Assert-ReleaseArchive([string]$zip, [string]$stage) {
  Add-Type -AssemblyName System.IO.Compression.FileSystem
  $expected = @{}
  foreach ($file in Get-ChildItem $stage -Recurse -File) {
    $expected[(Get-StageRelativePath $stage $file.FullName)] = $file.FullName
  }
  $archive = [System.IO.Compression.ZipFile]::OpenRead($zip)
  try {
    $seen = @{}
    foreach ($entry in $archive.Entries) {
      if ($entry.FullName.Contains('\')) {
        throw "release archive contains a non-portable Windows path: $($entry.FullName)"
      }
      $relative = $entry.FullName
      if ([string]::IsNullOrEmpty($entry.Name)) { continue }
      if ($relative.StartsWith('/') -or $relative -match '(^|/)\.\.(/|$)') {
        throw "release archive contains unsafe path: $relative"
      }
      if ($seen.ContainsKey($relative)) { throw "release archive contains duplicate path: $relative" }
      $seen[$relative] = $true
      if (-not $expected.ContainsKey($relative)) { throw "release archive contains unstaged payload: $relative" }
      $sha = [System.Security.Cryptography.SHA256]::Create()
      $stream = $entry.Open()
      try {
        $archiveHash = [BitConverter]::ToString($sha.ComputeHash($stream)).Replace('-', '')
      } finally {
        $stream.Dispose()
        $sha.Dispose()
      }
      if ($archiveHash -ne (Get-FileHash -Algorithm SHA256 -LiteralPath $expected[$relative]).Hash) {
        throw "release archive content differs from staging: $relative"
      }
    }
    $missing = @($expected.Keys | Where-Object { -not $seen.ContainsKey($_) })
    if ($missing.Count -ne 0) { throw "release archive is missing staged payload: $($missing -join ', ')" }
  } finally {
    $archive.Dispose()
  }
}

$stage = Join-Path $out 'stage'
if ([IO.Path]::GetFullPath($stage) -ne [IO.Path]::GetFullPath((Join-Path $root 'release\stage'))) {
  throw "refusing to use an unexpected release stage: $stage"
}
if (Test-Path -LiteralPath $stage) { Remove-Item -LiteralPath $stage -Recurse -Force }
New-Item -ItemType Directory -Force $stage | Out-Null

Copy-Item $exe $stage
$sdl = Join-Path $bin 'SDL2.dll'
if (-not (Test-Path $sdl -PathType Leaf)) { throw "missing required runtime file at $sdl" }
Copy-Item $sdl $stage

# The launcher resolves its fonts/images beside the executable. Copy the
# build-staged, console-filtered asset tree; never reach into a user cache.
$launcherAssets = Join-Path $bin 'assets'
if (-not (Test-Path $launcherAssets)) { throw "missing launcher assets at $launcherAssets" }
Copy-Item $launcherAssets (Join-Path $stage 'assets') -Recurse

# Built-in packages are source-controlled catalog data. Stage that pristine
# tree, never build_release\mods (which may hold a developer's state.toml).
$preloadedMods = Join-Path $root 'mods\preloaded'
if (-not (Test-Path $preloadedMods)) { throw "missing preloaded mod catalog at $preloadedMods" }
Copy-Item $preloadedMods (Join-Path $stage 'mods') -Recurse

$readme | Out-File -Encoding ascii (Join-Path $stage 'README.txt')
Assert-ReleaseStage $stage $preloadedMods

$zip = Join-Path $out 'LegendOfZeldaNESRecomp-windows-x64.zip'
if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip }
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem
$stageFull = [IO.Path]::GetFullPath($stage).TrimEnd('\') + '\'
$archive = [IO.Compression.ZipFile]::Open([IO.Path]::GetFullPath($zip), [IO.Compression.ZipArchiveMode]::Create)
try {
  foreach ($file in Get-ChildItem -LiteralPath $stage -Recurse -File | Sort-Object FullName) {
    $fileFull = [IO.Path]::GetFullPath($file.FullName)
    if (-not $fileFull.StartsWith($stageFull, [StringComparison]::OrdinalIgnoreCase)) {
      throw "refusing to archive a file outside the release stage: $fileFull"
    }
    $entryName = $fileFull.Substring($stageFull.Length).Replace('\', '/')
    [IO.Compression.ZipFileExtensions]::CreateEntryFromFile(
      $archive, $fileFull, $entryName, [IO.Compression.CompressionLevel]::Optimal) | Out-Null
  }
} finally {
  $archive.Dispose()
}
Assert-ReleaseArchive $zip $stage
Remove-Item -LiteralPath $stage -Recurse -Force
Write-Host "staged $zip"
