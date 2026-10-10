#!/usr/bin/env bash
set -euo pipefail
repo_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$repo_dir/build"
qt_option=OFF
if [[ "${1:-}" == "--qt" ]]; then
    build_dir="$repo_dir/build-qt"
    qt_option=ON
    shift
fi
cmake -S "$repo_dir" -B "$build_dir" -DCMAKE_BUILD_TYPE=Debug -DINTERFACE_LAB_QT="$qt_option" "$@"
cmake --build "$build_dir" --config Debug --parallel 2
ctest --test-dir "$build_dir" -C Debug --output-on-failure
printf '\nRun: "%s/bin/interface_lab" --all\n' "$build_dir"
