#!/usr/bin/env bash
set -euo pipefail
git submodule update --init --recursive nesrecomp recomp-ui
echo "Ready - pinned engine and UI initialized; cycle builds generate from your ROM."
