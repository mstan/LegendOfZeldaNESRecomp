<# Create local Zelda cycle preview archive with modern Mods. No publishing.
   powershell -File tools\make_release.ps1 -Rom "F:\ROMs\Legend of Zelda.NES"
   powershell -File tools\make_release.ps1 -SkipBuild
#>
param([string]$Rom, [switch]$SkipBuild, [string]$BuildDirectory, [string]$OutputDirectory,
      [string]$ReduxArchive, [string]$RemasteredPatch)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$build = if ($BuildDirectory) { [IO.Path]::GetFullPath($BuildDirectory) } else { Join-Path $root 'build-cycle' }
$out = if ($OutputDirectory) { [IO.Path]::GetFullPath($OutputDirectory) } else { Join-Path $root 'release' }
$cachePath = Join-Path $build 'CMakeCache.txt'
$cache = if (Test-Path -LiteralPath $cachePath) { Get-Content -LiteralPath $cachePath -Raw } else { '' }
function Get-CacheValue([string]$Name) {
    $match = [regex]::Match($cache, '(?m)^' + [regex]::Escape($Name) + ':[^=]+=(.*)\r?$')
    if ($match.Success) { return $match.Groups[1].Value.TrimEnd("`r") }
    return ''
}
function Invoke-HiddenBuild([string]$Executable, [string[]]$ToolArguments) {
    $quoted = foreach ($value in $ToolArguments) {
        $escaped = [regex]::Replace($value, '(\\*)"', '$1$1\"')
        $escaped = [regex]::Replace($escaped, '(\\+)$', '$1$1')
        '"' + $escaped + '"'
    }
    $info = New-Object Diagnostics.ProcessStartInfo
    $info.FileName = $Executable
    $info.Arguments = $quoted -join ' '
    $info.WorkingDirectory = $root
    $info.UseShellExecute = $false
    $info.CreateNoWindow = $true
    $info.WindowStyle = [Diagnostics.ProcessWindowStyle]::Hidden
    $info.RedirectStandardOutput = $true
    $info.RedirectStandardError = $true
    $process = New-Object Diagnostics.Process
    $process.StartInfo = $info
    try {
        $null = $process.Start()
        $stdout = $process.StandardOutput.ReadToEndAsync()
        $stderr = $process.StandardError.ReadToEndAsync()
        $process.WaitForExit()
        Write-Output $stdout.GetAwaiter().GetResult()
        Write-Output $stderr.GetAwaiter().GetResult()
        if ($process.ExitCode -ne 0) { throw "$Executable failed ($($process.ExitCode))" }
    } finally { $process.Dispose() }
}
if (-not $SkipBuild) {
    if (-not $Rom) { $Rom = Get-CacheValue 'NESRECOMP_ROM' }
    if (-not $Rom -or -not (Test-Path -LiteralPath $Rom -PathType Leaf)) { throw 'Supply a verified USA PRG0 ROM with -Rom.' }
    $Rom = (Resolve-Path -LiteralPath $Rom).Path
    $cmake = Get-CacheValue 'CMAKE_COMMAND'
    if (-not $cmake) { $cmake = 'C:\Program Files\CMake\bin\cmake.exe' }
    if (-not (Test-Path -LiteralPath $cmake)) { $cmake = (Get-Command cmake.exe -ErrorAction Stop).Source }
    $args = @('-S', $root, '-B', $build, '-DNESRECOMP_BACKEND=cycle', "-DNESRECOMP_ROM=$Rom")
    if (-not $ReduxArchive) { $ReduxArchive = Get-CacheValue 'ZELDA_REDUX_ARCHIVE' }
    if (-not $RemasteredPatch) { $RemasteredPatch = Get-CacheValue 'ZELDA_REMASTERED_PATCH' }
    if ($ReduxArchive) {
        if (-not $RemasteredPatch) { throw 'Redux builds require the supported Remastered patch.' }
        $args += "-DZELDA_REDUX_ARCHIVE=$((Resolve-Path -LiteralPath $ReduxArchive).Path)"
        $args += "-DZELDA_REMASTERED_PATCH=$((Resolve-Path -LiteralPath $RemasteredPatch).Path)"
    }
    Invoke-HiddenBuild $cmake $args
    Invoke-HiddenBuild $cmake @('--build', $build, '--config', 'Release', '--parallel', '1')
    $cache = Get-Content -LiteralPath $cachePath -Raw
}
if ((Get-CacheValue 'NESRECOMP_BACKEND') -ne 'cycle') { throw 'Refusing to package a legacy build.' }
$bin = if (Test-Path -LiteralPath (Join-Path $build 'Release\LegendOfZeldaNESRecomp.exe')) { Join-Path $build 'Release' } else { $build }
New-Item -ItemType Directory -Force -Path $out | Out-Null
$variants = @('LegendOfZeldaNESRecomp')
foreach ($target in $variants) {
    foreach ($dependency in "$target.exe", 'SDL2.dll', 'assets') {
        if (-not (Test-Path -LiteralPath (Join-Path $bin $dependency))) { throw "Missing release dependency: $dependency" }
    }
    $stage = Join-Path $out ('stage-' + [guid]::NewGuid().ToString('N'))
    New-Item -ItemType Directory -Path $stage | Out-Null
    try {
        foreach ($file in "$target.exe", 'SDL2.dll') { Copy-Item -LiteralPath (Join-Path $bin $file) -Destination $stage }
        $compiler = Get-CacheValue 'CMAKE_C_COMPILER'
        $mingw = [IO.Path]::GetFileName($compiler) -match '(^|-)gcc(?:-[0-9.]+)?\.exe$'
        foreach ($dll in 'libgcc_s_seh-1.dll', 'libstdc++-6.dll', 'libwinpthread-1.dll') {
            $runtime = Join-Path $bin $dll
            if (-not (Test-Path -LiteralPath $runtime) -and $mingw) {
                $runtime = Join-Path (Split-Path -Parent $compiler) $dll
            }
            if (Test-Path -LiteralPath $runtime) { Copy-Item -LiteralPath $runtime -Destination $stage }
            elseif ($mingw) { throw "Missing MinGW runtime dependency: $dll" }
        }
        Copy-Item -LiteralPath (Join-Path $bin 'assets') -Recurse -Destination (Join-Path $stage 'assets')
        Copy-Item -LiteralPath (Join-Path $root 'mods\preloaded') -Recurse -Destination (Join-Path $stage 'mods')
        if (Get-CacheValue 'ZELDA_REDUX_ARCHIVE') {
            $reduxPackage = Join-Path $build 'redux/mods/packages/legend-of-zelda.redux/3.3.3'
            foreach ($asset in 'manifest.toml', 'Zelda1_Redux.ips', 'optional/Like Like Rupees.ips',
                     'optional/Zelda 1 & A New Light Dungeon Themes.ips', 'optional/Reworked Title Screen.ips',
                     'ReadMe.md', 'LICENSE', 'Zelda1-Redux-source.zip') {
                if (-not (Test-Path -LiteralPath (Join-Path $reduxPackage $asset) -PathType Leaf)) {
                    throw "Missing compiled Redux package asset: $asset"
                }
            }
            $reduxTarget = Join-Path $stage 'mods/packages/legend-of-zelda.redux'
            New-Item -ItemType Directory -Path $reduxTarget | Out-Null
            Copy-Item -LiteralPath $reduxPackage -Recurse -Destination $reduxTarget
        }
        New-Item -ItemType Directory -Path (Join-Path $stage 'tools') | Out-Null
        New-Item -ItemType Directory -Path (Join-Path $stage 'hdpatch') | Out-Null
        Copy-Item -LiteralPath (Join-Path $root 'tools\import_hdpack.py') -Destination (Join-Path $stage 'tools')
        $framework = Get-CacheValue 'NESRECOMP_ROOT'
        if (-not $framework) { $framework = Join-Path $root 'nesrecomp' }
        Copy-Item -LiteralPath (Join-Path $framework 'tools\package_hdpack.py') -Destination (Join-Path $stage 'tools')
        Copy-Item -LiteralPath (Join-Path $root 'hdpatch\ZeldaRemasteredReadme.txt') -Destination (Join-Path $stage 'hdpatch')
        foreach ($doc in 'README.md', 'LICENSE', 'PERMISSIONS.md') { Copy-Item -LiteralPath (Join-Path $root $doc) -Destination $stage }
        $ui = Get-CacheValue 'NESRECOMP_RECOMP_UI'
        if (-not $ui) { $ui = Get-CacheValue 'RECOMP_UI_ROOT' }
        if (-not $ui) { $ui = Join-Path $root 'recomp-ui' }
        $licenses = @{
            'Game-LICENSE.txt' = (Join-Path $root 'LICENSE')
            'Game-PERMISSIONS.md' = (Join-Path $root 'PERMISSIONS.md')
            'NESRecomp-LICENSE.txt' = (Join-Path $framework 'LICENSE')
            'SDL2-COPYING.txt' = (Join-Path $framework 'runner/external/SDL2/COPYING.txt')
            'ImGui-LICENSE.txt' = (Join-Path $ui 'src/third_party/imgui/LICENSE.txt')
        }
        New-Item -ItemType Directory -Path (Join-Path $stage 'licenses') | Out-Null
        foreach ($name in $licenses.Keys) {
            if (-not (Test-Path -LiteralPath $licenses[$name])) { throw "Missing license: $($licenses[$name])" }
            Copy-Item -LiteralPath $licenses[$name] -Destination (Join-Path $stage "licenses/$name")
        }
        New-Item -ItemType Directory -Path (Join-Path $stage 'docs\assets') | Out-Null
        foreach ($doc in 'CYCLE-MIGRATION.md', 'RELEASE_NOTES.md', 'DIAGNOSTICS.md', 'SEAMLESS-WORLD.md') {
            Copy-Item -LiteralPath (Join-Path $root "docs\$doc") -Destination (Join-Path $stage 'docs')
        }
        foreach ($image in 'seamless-overworld.png', 'seamless-dungeon.png', 'seamless-atlas.png', 'voxel-3d.webp') {
            Copy-Item -LiteralPath (Join-Path $root "docs\assets\$image") -Destination (Join-Path $stage 'docs\assets')
        }
        $symbolFile = Join-Path $bin "$target.pdb"
        $splitSymbols = $false
        if (-not (Test-Path -LiteralPath $symbolFile)) {
            $objcopy = Get-CacheValue 'CMAKE_OBJCOPY'
            if (-not $objcopy -or -not (Test-Path -LiteralPath $objcopy) -or
                (Get-CacheValue 'CMAKE_C_FLAGS_RELEASE') -notmatch '(?<!\S)-g\d*(?!\S)') {
                throw 'Build with matching PDBs or GNU debug information before packaging.'
            }
            $symbolFile = Join-Path $out "$target.exe.debug"
            $splitSymbols = $true
            $stagedExe = Join-Path $stage "$target.exe"
            Invoke-HiddenBuild $objcopy @('--only-keep-debug', $stagedExe, $symbolFile)
            Invoke-HiddenBuild $objcopy @('--strip-debug', $stagedExe)
            Invoke-HiddenBuild $objcopy @("--add-gnu-debuglink=$symbolFile", $stagedExe)
        }
        $readme = @'
The Legend of Zelda - USA/NTSC cycle preview

Optional Redux 3.3.3 builds include Redux and its three companion Mods, all
initially disabled. Select them before Play and restart to change them.
Redux has separate battery progress shared by its companion combinations.
The compatible Remastered1.0.6 pack supplies adapted graphics with the Redux
HUD/menus/new art preserved. Redux uses its native viewport; Seamless Overworld
requires the stock program. No Redux widescreen support is claimed.

v1.12.1 fixes Player 1 keyboard remaps entered in the launcher.
They are now saved in the active game settings and used during gameplay.

v1.12.0 adds Stopwatch scope under Seamless Overworld's Gameplay options:
Entire map (the existing default), Current viewport, or Current room.
Viewport and room are captured at pickup and stay fixed when moving,
zooming or resizing. Another clock captures a new area; save states retain it.
Fresh profiles enable Seamless Overworld for adaptive widescreen by default.
Existing explicit display/mod choices, native saves and settings are preserved.

v1.11.0 adds resident GPU rendering for Zelda Remastered HD. Graphics > HD
resolution selects Pack resolution or 1x, 2x, 4x, 8x and 16x; 32x and 64x are
experimental. The qualified RTX 3080 Ti gameplay interval reached 93.24-108.30
FPS at true 16x (4096x3840), with no GPU fallback. Portable CPU rendering uses
the pack's authored resolution when the GPU backend is unavailable. Stock
and voxel views retain their existing renderers. See docs/RELEASE_NOTES.md.

v1.10.1 fixes save-state loading after switching from voxel views to stock or
Seamless Overworld. Disabled voxel camera settings no longer block a load.
Active voxel states still require the matching voxel mode and camera preset.

v1.10.0 adds Seamless Overworld (Experimental), first in Mods under Camera.
Enable it before PLAY for a continuous camera, neighboring terrain and live
actors, adaptive Fit to screen and connected dungeon rooms. Mouse wheel changes
pixel zoom; F4 shows the whole map. It is bundled; fresh profiles enable it.
Caves, stairs and maze puzzles preserve their original portal routing.
Local play only; cross-room enemy/projectile interactions remain incomplete,
whole-map simulation is slower, and broader full-game coverage remains open.
README.md includes real overworld, dungeon and whole-map pictures.

No ROM or third-party HD art is included. Select your stock USA PRG0 ROM,
payload CRC32 3fe272fb, for every mode. Remastered is a modern Mod in the same
executable. Make a local .nesmod from your existing pack and stock ROM with:
python tools/import_hdpack.py --pack "path/to/pack" --rom "path/to/stock.nes" --out "Zelda-Remastered.nesmod"
Install it in Mods, then enable Zelda Remastered HD before Play. The shared
framework verifies and applies its IPS in memory. Original files stay intact.
Remastered supports replacement audio. Optional Redux uses its native game audio.

Arrow keys: D-pad. Z: A. X: B. Enter: Start. Backslash: Select.
Escape: menu/settings. Hold Tab: fast-forward. F7: state slots. F8: rewind.
F11: fullscreen. F12: screenshot. Gamepads and remapping are supported.
Older raw 8 KiB battery progress imports through the launcher SAVE panel.
Legacy binary save states cannot transfer to this backend. --no-save disables
battery loading and writes. Copy original progress before experimenting.

Stock's Voxel 3D and first-person packages are disabled by default and mutually
exclusive. Numpad 0 toggles the view; 8/2 pitch, 4/6 yaw, 7/9 roll, +/- zoom,
1/3 sprite scale and 5 resets the camera. First-person movement follows the
camera; the right stick looks. HD is mutually exclusive with voxel views. Disable HD in the launcher and
restart to return to stock. Save states require matching pack assets/patch.
These builds are development previews. Read docs/CYCLE-MIGRATION.md for coverage
and existing presentation limitations.

For a startup crash: open Mods and enable Startup and Crash Diagnostics,
then click PLAY and reproduce the issue. Share the newest .log and matching
.dmp, if present, from the diagnostics folder beside the program. Disable the
mod after testing. See docs/DIAGNOSTICS.md for fallback storage and privacy details.
'@
        [IO.File]::WriteAllText((Join-Path $stage 'README.txt'), $readme, [Text.Encoding]::UTF8)
        $forbidden = @(Get-ChildItem -LiteralPath $stage -File -Recurse | Where-Object {
            $_.Extension -in '.nes', '.srm', '.sav', '.state', '.cycstate', '.log', '.dmp' -or
            $_.Name -in 'config.ini', 'keybinds.ini', 'debug.ini', 'rom.cfg', 'state.toml', 'hires.txt'
        })
        if ($forbidden.Count) { throw 'Player/debug/third-party data found in staging.' }
        $zip = Join-Path $out "$target-USA-NTSC-cycle-mods-preview-windows-x64.zip"
        if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip }
        Compress-Archive -Path (Join-Path $stage '*') -DestinationPath $zip
        Write-Host "Created local preview: $zip"
        Write-Host "SHA256: $((Get-FileHash -LiteralPath $zip -Algorithm SHA256).Hash)"
        $symbols = Join-Path $out "$target-windows-x64-symbols.zip"
        if (Test-Path -LiteralPath $symbols) { Remove-Item -LiteralPath $symbols }
        Compress-Archive -LiteralPath $symbolFile -DestinationPath $symbols
        Write-Host "Created matching crash symbols: $symbols"
        if ($splitSymbols) { Remove-Item -LiteralPath $symbolFile -ErrorAction Stop }
    } finally {
        $stageAbsolute = [IO.Path]::GetFullPath($stage)
        $outPrefix = [IO.Path]::GetFullPath($out).TrimEnd('\') + '\'
        if (-not $stageAbsolute.StartsWith($outPrefix, [StringComparison]::OrdinalIgnoreCase)) { throw "Refusing cleanup outside release directory: $stageAbsolute" }
        $contents = @(Get-ChildItem -LiteralPath $stageAbsolute -Recurse -Force)
        if (((Get-Item -LiteralPath $stageAbsolute).Attributes -band [IO.FileAttributes]::ReparsePoint) -or
            @($contents | Where-Object { ($_.Attributes -band [IO.FileAttributes]::ReparsePoint) -or $_.Name -in '.git', '.hg', '.svn' }).Count) {
            throw "Unexpected link or repository in owned release staging: $stageAbsolute"
        }
        Remove-Item -LiteralPath $stageAbsolute -Recurse -ErrorAction Stop
    }
}
