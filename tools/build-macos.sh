#!/usr/bin/env bash
# Title policy only; platform orchestration belongs to the pinned engine.
set -euo pipefail
REPO="$(cd "$(dirname "$0")/.." && pwd)"
NES_ENGINE="${NESRECOMP_ROOT:-$REPO/nesrecomp}"
APP_NAME="ZeldaNES"
CMAKE_TARGET="LegendOfZeldaNESRecomp"
ROM_EXTS="nes"
EXTRA_ARGS=""
REGEN_CMD=""
PREBUILD_CMD=""
POSTBUILD_CMD=""
PROD_CMAKE_FLAGS=( -DNESRECOMP_ENABLE_TRACE=OFF )
DEBUG_CMAKE_FLAGS=( -DNESRECOMP_ENABLE_TRACE=ON )
BUNDLE_ID="com.mstan.zeldanesrecomp"
nes_platform_configure() {
    [ -f "${NESRECOMP_ROM:-}" ] || { echo 'Supply the USA PRG0 image with --rom PATH.' >&2; exit 2; }
}
source "$NES_ENGINE/tools/build_cycle_macos.sh" "$@"
