<# Create local Zelda stock/Remastered cycle preview archives. No publishing.
   powershell -File tools\make_release.ps1 -Rom "F:\ROMs\Legend of Zelda.NES" [-HdRom "F:\ROMs\zelda_hd.nes"]
   powershell -File tools\make_release.ps1 -SkipBuild
#>
param([string]$Rom, [string]$HdRom, [switch]$SkipBuild)
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
    if ($HdRom) { $args += "-DZELDA_HD_ROM=$((Resolve-Path -LiteralPath $HdRom).Path)" }
    Invoke-HiddenBuild $cmake $args
    Invoke-HiddenBuild $cmake @('--build', $build, '--config', 'Release')
    $cache = Get-Content -LiteralPath $cachePath -Raw
}
if ((Get-CacheValue 'NESRECOMP_BACKEND') -ne 'cycle') { throw 'Refusing to package a legacy build.' }
New-Item -ItemType Directory -Force -Path $out | Out-Null
$variants = @('LegendOfZeldaNESRecomp')
if (Get-CacheValue 'ZELDA_HD_ROM') { $variants += 'LegendOfZeldaNESRecomp-HD' }
foreach ($target in $variants) {
    foreach ($dependency in "$target.exe", 'SDL2.dll', 'assets') {
        if (-not (Test-Path -LiteralPath (Join-Path $bin $dependency))) { throw "Missing release dependency: $dependency" }
    }
    $stage = Join-Path $out ('stage-' + [guid]::NewGuid().ToString('N'))
    New-Item -ItemType Directory -Path $stage | Out-Null
    try {
        foreach ($file in "$target.exe", 'SDL2.dll') { Copy-Item -LiteralPath (Join-Path $bin $file) -Destination $stage }
        Copy-Item -LiteralPath (Join-Path $bin 'assets') -Recurse -Destination (Join-Path $stage 'assets')
        if ($target -eq 'LegendOfZeldaNESRecomp') {
            Copy-Item -LiteralPath (Join-Path $root 'mods\preloaded') -Recurse -Destination (Join-Path $stage 'mods')
            $manifests = @(Get-ChildItem -LiteralPath (Join-Path $stage 'mods\packages') -Filter manifest.toml -Recurse -File)
            if ($manifests.Count -ne 2) { throw 'Expected the two stock voxel packages.' }
            foreach ($manifest in $manifests) {
                if ((Get-Content -LiteralPath $manifest.FullName -Raw) -notmatch '(?m)^rom_crc32\s*=\s*"3fe272fb"\s*$') { throw 'Voxel packages must target stock USA PRG0.' }
            }
        } else {
            New-Item -ItemType Directory -Path (Join-Path $stage 'tools') | Out-Null
            New-Item -ItemType Directory -Path (Join-Path $stage 'hdpatch') | Out-Null
            Copy-Item -LiteralPath (Join-Path $root 'tools\apply_hd_patch.py') -Destination (Join-Path $stage 'tools')
            Copy-Item -LiteralPath (Join-Path $root 'hdpatch\ZeldaHD.ips') -Destination (Join-Path $stage 'hdpatch')
        }
        foreach ($doc in 'CYCLE-MIGRATION.md', 'RELEASE_NOTES.md', 'LICENSE') { Copy-Item -LiteralPath (Join-Path $root $doc) -Destination $stage }
        $readme = @'
The Legend of Zelda - USA/NTSC cycle preview

No ROM or third-party HD assets are included. Stock uses USA PRG0, headerless
PRG+CHR CRC32 3fe272fb. Remastered uses the locally IPS-patched derivative,
CRC32 fd9c577f; create it with tools/apply_hd_patch.py and your matching stock
ROM, then select that derivative in the HD executable. Point its HD settings
at your local pack folder containing hires.txt. Replacement HD audio is not
implemented; the original NES APU continues playing the patched guest audio.

Arrow keys: D-pad. Z: A. X: B. Enter: Start. Backslash: Select.
Escape: menu/settings. Hold Tab: fast-forward. F8/F9: cycle save/load state.
F11: fullscreen. F12: screenshot. Gamepads and remapping are supported.
Older raw 8 KiB battery progress imports through the launcher SAVE panel.
Legacy binary save states cannot transfer to this backend. --no-save disables
battery loading and writes. Copy original progress before experimenting.

Stock's Voxel 3D and first-person packages are disabled by default and mutually
exclusive. Numpad 0 toggles the view; 8/2 pitch, 4/6 yaw, 7/9 roll, +/- zoom,
1/3 sprite scale and 5 resets the camera. First-person movement follows the
camera; the right stick looks. The HD target has no stock voxel packages.
These builds are development previews. Read CYCLE-MIGRATION.md for coverage
and existing presentation limitations.
'@
        [IO.File]::WriteAllText((Join-Path $stage 'README.txt'), $readme, [Text.Encoding]::UTF8)
        $forbidden = @(Get-ChildItem -LiteralPath $stage -File -Recurse | Where-Object {
            $_.Extension -in '.nes', '.srm', '.sav', '.state', '.cycstate', '.log' -or
            $_.Name -in 'config.ini', 'keybinds.ini', 'debug.ini', 'rom.cfg', 'state.toml', 'hires.txt'
        })
        if ($forbidden.Count) { throw 'Player/debug/third-party data found in staging.' }
        $zip = Join-Path $out "$target-USA-NTSC-cycle-preview-windows-x64.zip"
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
