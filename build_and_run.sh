#!/usr/bin/env bash
set -e

BUILD_TESTS=OFF

while getopts "t" opt; do
  case "$opt" in
    t) BUILD_TESTS=ON ;;
    *) echo "Usage: $0 [-t]" && exit 1 ;;
  esac
done

rm -rf build
mkdir build
cd build

cmake .. -DBUILD_TESTS=${BUILD_TESTS}
make

if [[ "${BUILD_TESTS}" == "ON" ]]; then
  echo "Running unit tests…"
  ./WordfeudHelperTests
else
  echo "Running main app…"
  ./WordfeudHelper
fi
