#!/bin/bash
# Golden-master check: proves a refactor did not change behaviour.
#
#   ./run.sh           build the firmware against the mock Arduino, run every
#                      recorded scenario and compare with golden_hashes.txt
#   ./run.sh --update  re-record golden_hashes.txt (only after an INTENDED
#                      behaviour change)
#
# Needs only g++ (C++17).
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$HERE/../.." && pwd)"
SUMO_SRC="$ROOT/Sumo/2024-2025/Sumolatest/src"
LINE_SKETCH="$ROOT/Line_tracker/2024/Line_tracker.ino"
GOLDEN="$HERE/golden_hashes.txt"
BUILD="$(mktemp -d)"
trap 'rm -rf "$BUILD"' EXIT

CXX="${CXX:-g++}"
FLAGS=(-std=c++17 -O1 -w -I"$HERE/arduino_mock" -I"$HERE")

echo "Building..."
"$CXX" "${FLAGS[@]}" -I"$SUMO_SRC" "$HERE/sumo_harness.cpp" "$SUMO_SRC"/*.cpp \
  "$HERE/arduino_mock/mock.cpp" -o "$BUILD/sumo"
"$CXX" "${FLAGS[@]}" -DSKETCH="\"$LINE_SKETCH\"" "$HERE/line_tracker_harness.cpp" \
  "$HERE/arduino_mock/mock.cpp" -o "$BUILD/line"

run_case() { # <program> <mode> <seed>
  if [ "$1" = sumo ]; then "$BUILD/sumo" "$3" "$2"; else "$BUILD/line" "$3"; fi
}

if [ "${1:-}" = "--update" ]; then
  : > "$GOLDEN"
  for mode in main simple smart prim; do
    for seed in $(seq 1 50); do echo "sumo $mode $seed $(run_case sumo "$mode" "$seed")" >> "$GOLDEN"; done
  done
  for seed in $(seq 1 50); do echo "line - $seed $(run_case line - "$seed")" >> "$GOLDEN"; done
  echo "Recorded $(wc -l < "$GOLDEN") scenarios in $GOLDEN"
  exit 0
fi

echo "Running scenarios..."
failures=0
total=0
while read -r program mode seed hash count; do
  total=$((total + 1))
  actual="$(run_case "$program" "$mode" "$seed")"
  if [ "$actual" != "$hash $count" ]; then
    echo "CHANGED: $program mode=$mode seed=$seed (expected $hash $count, got $actual)"
    failures=$((failures + 1))
  fi
done < "$GOLDEN"

if [ "$failures" -eq 0 ]; then
  echo "OK: all $total scenarios behave exactly as recorded."
else
  echo "FAILED: $failures of $total scenarios behave differently."
  exit 1
fi
