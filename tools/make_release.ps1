<# Create local Zelda cycle preview archive with modern Mods. No publishing.
   powershell -File tools\make_release.ps1 -Rom "F:\ROMs\Legend of Zelda.NES"
   powershell -File tools\make_release.ps1 -SkipBuild
#>
param([string]$Rom, [switch]$SkipBuild)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath((Split-Path -Parent $PSScriptRoot))
$build = Join-Path $root 'build-cycle'
$bin = Join-Path $build 'Release'
$out = Join-Path $root 'release'
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
    $cmake = 'C:\Program Files\CMake\bin\cmake.exe'
    if (-not (Test-Path -LiteralPath $cmake)) { $cmake = (Get-Command cmake.exe -ErrorAction Stop).Source }
    $args = @('-S', $root, '-B', $build, '-DNESRECOMP_BACKEND=cycle', "-DNESRECOMP_ROM=$Rom")
    Invoke-HiddenBuild $cmake $args
    Invoke-HiddenBuild $cmake @('--build', $build, '--config', 'Release')
    $cache = Get-Content -LiteralPath $cachePath -Raw
}
if ((Get-CacheValue 'NESRECOMP_BACKEND') -ne 'cycle') { throw 'Refusing to package a legacy build.' }
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
        Copy-Item -LiteralPath (Join-Path $bin 'assets') -Recurse -Destination (Join-Path $stage 'assets')
        Copy-Item -LiteralPath (Join-Path $root 'mods\preloaded') -Recurse -Destination (Join-Path $stage 'mods')
        New-Item -ItemType Directory -Path (Join-Path $stage 'tools') | Out-Null
        New-Item -ItemType Directory -Path (Join-Path $stage 'hdpatch') | Out-Null
        Copy-Item -LiteralPath (Join-Path $root 'tools\import_hdpack.py') -Destination (Join-Path $stage 'tools')
        $framework = Get-CacheValue 'NESRECOMP_ROOT'
        if (-not $framework) { $framework = Join-Path $root 'nesrecomp' }
        Copy-Item -LiteralPath (Join-Path $framework 'tools\package_hdpack.py') -Destination (Join-Path $stage 'tools')
        Copy-Item -LiteralPath (Join-Path $root 'hdpatch\ZeldaRemasteredReadme.txt') -Destination (Join-Path $stage 'hdpatch')
        foreach ($doc in 'CYCLE-MIGRATION.md', 'RELEASE_NOTES.md', 'LICENSE') { Copy-Item -LiteralPath (Join-Path $root $doc) -Destination $stage }
        $readme = @'
The Legend of Zelda - USA/NTSC cycle preview

No ROM or third-party HD art is included. Select your stock USA PRG0 ROM,
payload CRC32 3fe272fb, for every mode. Remastered is a modern Mod in the same
executable. Make a local .nesmod from your existing pack and stock ROM with:
python tools/import_hdpack.py --pack "path/to/pack" --rom "path/to/stock.nes" --out "Zelda-Remastered.nesmod"
Install it in Mods, then enable Zelda Remastered HD before Play. The shared
framework verifies and applies its IPS in memory. Original files stay intact.
Remastered replacement audio is not implemented; the original NES APU plays.

Arrow keys: D-pad. Z: A. X: B. Enter: Start. Backslash: Select.
Escape: menu/settings. Hold Tab: fast-forward. F8/F9: cycle save/load state.
F11: fullscreen. F12: screenshot. Gamepads and remapping are supported.
Older raw 8 KiB battery progress imports through the launcher SAVE panel.
Legacy binary save states cannot transfer to this backend. --no-save disables
battery loading and writes. Copy original progress before experimenting.

Stock's Voxel 3D and first-person packages are disabled by default and mutually
exclusive. Numpad 0 toggles the view; 8/2 pitch, 4/6 yaw, 7/9 roll, +/- zoom,
1/3 sprite scale and 5 resets the camera. First-person movement follows the
camera; the right stick looks. HD is mutually exclusive with voxel views. Disable HD in the launcher and
restart to return to stock. Save states require matching pack assets/patch.
These builds are development previews. Read CYCLE-MIGRATION.md for coverage
and existing presentation limitations.
'@
        [IO.File]::WriteAllText((Join-Path $stage 'README.txt'), $readme, [Text.Encoding]::UTF8)
        $forbidden = @(Get-ChildItem -LiteralPath $stage -File -Recurse | Where-Object {
            $_.Extension -in '.nes', '.srm', '.sav', '.state', '.cycstate', '.log' -or
            $_.Name -in 'config.ini', 'keybinds.ini', 'debug.ini', 'rom.cfg', 'state.toml', 'hires.txt'
        })
        if ($forbidden.Count) { throw 'Player/debug/third-party data found in staging.' }
        $zip = Join-Path $out "$target-USA-NTSC-cycle-mods-preview-windows-x64.zip"
        if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip }
        Compress-Archive -Path (Join-Path $stage '*') -DestinationPath $zip
        Write-Host "Created local preview: $zip"
        Write-Host "SHA256: $((Get-FileHash -LiteralPath $zip -Algorithm SHA256).Hash)"
    } finally {
        $stageAbsolute = [IO.Path]::GetFullPath($stage)
        $outPrefix = [IO.Path]::GetFullPath($out).TrimEnd('\') + '\'
        if (-not $stageAbsolute.StartsWith($outPrefix, [StringComparison]::OrdinalIgnoreCase)) { throw "Refusing cleanup outside release directory: $stageAbsolute" }
        Remove-Item -LiteralPath $stageAbsolute -Recurse -Force
    }
}
