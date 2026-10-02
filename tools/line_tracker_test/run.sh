#!/bin/bash
# Builds and runs the line tracker regression tests. Needs only g++ (C++17).
set -euo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
SKETCH_DIR="$HERE/../../Line_tracker/2024"
BUILD="$(mktemp -d)"
trap 'rm -rf "$BUILD"' EXIT
"${CXX:-g++}" -std=c++17 -w -I"$SKETCH_DIR" "$HERE/test.cpp" -o "$BUILD/test"
"$BUILD/test"
