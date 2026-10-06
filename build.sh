#!/usr/bin/env bash
set -e

if ! command -v emcmake &>/dev/null; then
  if [ -f "/etc/profile.d/emscripten.sh" ]; then
    source /etc/profile.d/emscripten.sh
  elif [ -n "$EMSDK" ]; then
    source "$EMSDK/emsdk_env.sh" &>/dev/null
  fi
fi

if ! command -v emcmake &>/dev/null; then
  echo "Error: 'emcmake' not found in PATH."
  echo "Please activate emsdk environment first (e.g., 'source /path/to/emsdk/emsdk_env.sh')."
  exit 1
fi

BUILD_DIR="build"
BUILD_TYPE="${1:-Release}"

echo "=== Creating build directory for ($BUILD_TYPE) ==="
mkdir -p "$BUILD_DIR"

echo "=== Running CMake with Emscripten ==="
emcmake cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo "=== Compiling Project ==="
cmake --build "$BUILD_DIR" --parallel

echo ""
echo "=== Build Complete! ==="
echo "Clean deployment files generated in './dist/'"
echo ""
echo "To test locally, run:"
echo "  emrun dist/index.html"
