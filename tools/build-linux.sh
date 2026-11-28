#!/usr/bin/env bash
# Title policy only; platform orchestration belongs to the pinned engine.
set -euo pipefail
REPO="$(cd "$(dirname "$0")/.." && pwd)"
NES_ENGINE="${NESRECOMP_ROOT:-$REPO/nesrecomp}"
APP_NAME="ZeldaNES"
CMAKE_TARGET="LegendOfZeldaNESRecomp"
ROM_EXTS="nes"                             # AppRun auto-finds *.nes next to the AppImage
EXTRA_ARGS=""
REGEN_CMD=""                               # generated C is committed; regen is the Windows recompiler
PREBUILD_CMD=""
POSTBUILD_CMD=""
# prod/debug -> framework cmake flags. NESRECOMP_ENABLE_TRACE OFF strips the TCP
# server from production builds.
PROD_CMAKE_FLAGS=( -DNESRECOMP_ENABLE_TRACE=OFF )
DEBUG_CMAKE_FLAGS=( -DNESRECOMP_ENABLE_TRACE=ON )
nes_platform_configure() {
    [ -f "${NESRECOMP_ROM:-}" ] || { echo 'Supply the USA PRG0 image with --rom PATH.' >&2; exit 2; }
}
source "$NES_ENGINE/tools/build_cycle_linux.sh" "$@"
