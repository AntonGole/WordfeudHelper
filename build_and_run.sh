#!/usr/bin/env bash
set -e

BUILD_TESTS=OFF
RUN_PYTHON=OFF
RUN_ALL=OFF

while [[ "$#" -gt 0 ]]; do
  case "$1" in
    -t) BUILD_TESTS=ON ;;
    -p) RUN_PYTHON=ON ;;
    -all) RUN_ALL=ON ;;
    *) echo "Usage: $0 [-p] [-t] [-all]" && exit 1 ;;
  esac
  shift
done

if [[ "$RUN_PYTHON" == "ON" || "$RUN_ALL" == "ON" ]]; then
  echo "Running Python read_board.py..."
  source venv/bin/activate
  python3 read_board.py
  deactivate
fi

if [[ "$RUN_PYTHON" == "ON" && "$RUN_ALL" != "ON" ]]; then
  exit 0
fi

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
