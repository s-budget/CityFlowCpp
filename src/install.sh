#!/bin/bash
set -e

echo "========================================"
echo "CityFlow Library Installation (Linux)"
echo "========================================"
echo ""

# Default install location and build type
DEFAULT_INSTALL_PATH="$HOME/local/cityflowcpp"
DEFAULT_BUILD_TYPE="Release"

INSTALL_PATH="${1:-$DEFAULT_INSTALL_PATH}"
BUILD_TYPE="${2:-$DEFAULT_BUILD_TYPE}"

# Validate build type
case "$BUILD_TYPE" in
    Release|Debug|RelWithDebInfo|MinSizeRel)
        ;;
    *)
        echo "Error: Invalid build type '$BUILD_TYPE'"
        echo "Valid options: Release, Debug, RelWithDebInfo, MinSizeRel"
        exit 1
        ;;
esac

echo "Install location: $INSTALL_PATH"
echo "Build type: $BUILD_TYPE"
echo ""
read -p "Continue with installation? (y/n): " -n 1 -r
echo ""

if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Installation cancelled."
    exit 0
fi

# Detect number of CPU cores for parallel build
if [[ "$OSTYPE" == "darwin"* ]]; then
    CORES=$(sysctl -n hw.ncpu)
else
    CORES=$(nproc)
fi

echo ""
echo "Creating build directory..."
if [ -d "build" ]; then
    echo "Cleaning old build directory..."
    rm -rf build
fi
mkdir build
cd build

echo ""
echo "Configuring CMake..."
cmake -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" \
      -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
      ..

echo ""
echo "Building CityFlow (using $CORES cores)..."
make -j$CORES

echo ""
echo "Installing CityFlow..."
make install

echo ""
echo "========================================"
echo "Installation completed successfully!"
echo "========================================"
echo ""
echo "CityFlow installed to: $INSTALL_PATH"
echo "Build type: $BUILD_TYPE"
echo ""