#!/bin/bash
set -e  # Exit on error

# Define CMake version and path
CMAKE_VERSION="3.29.0"
CMAKE_DIR="$HOME/.cmake-bin"
CMAKE_BINARY="$CMAKE_DIR/bin/cmake"

# Function to download and install CMake if missing
install_cmake() {
    echo "🔧 CMake not found, downloading..."
    mkdir -p "$CMAKE_DIR"
    curl -LO "https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION-linux-x86_64.tar.gz"
    tar -xzf "cmake-$CMAKE_VERSION-linux-x86_64.tar.gz" -C "$CMAKE_DIR" --strip-components=1
    rm "cmake-$CMAKE_VERSION-linux-x86_64.tar.gz"
}

# Check if CMake exists, if not, install it
if ! [ -x "$CMAKE_BINARY" ]; then
    install_cmake
fi

# Verify installation
if ! [ -x "$CMAKE_BINARY" ]; then
    echo "❌ CMake installation failed. Please check manually."
    exit 1
fi

# Remove old build directory
rm -rf build

# Create a clean build directory
mkdir -p build
cd build

# Run CMake (ensuring it stays inside build/)
"$CMAKE_BINARY" ..
"$CMAKE_BINARY" --build .

echo "✅ Build completed!"
