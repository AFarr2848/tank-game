#!/usr/bin/env bash
set -e

if ! command -v emcmake &>/dev/null; then
  source /etc/profile.d/emscripten.sh
  echo "Error: 'emcmake' not found in PATH."
  echo "Please activate emsdk environment first (e.g., 'source /path/to/emsdk/emsdk_env.sh')."
  exit 1
fi

BUILD_DIR="build"

echo "=== Creating build directory ==="
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "=== Running CMake with Emscripten ==="
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release

echo "=== Compiling Project ==="
emmake make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ -f "tank-game.html" ]; then
  cp tank-game.html index.html
fi

echo ""
echo "=== Build Complete! ==="
echo "Generated files in './$BUILD_DIR'"
echo ""
echo "To test locally, run:"
echo "  emrun build/index.html"
