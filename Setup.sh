#!/bin/bash

# Call the install-vulkan.sh script
echo "Running install-vulkan.sh..."
./Scripts/install-vulkan.sh

# Perform CMake submodule initialization
echo "Initializing CMake submodules..."
cmake -B build -S . -DBUILD_TESTS=OFF

# Build your project
echo "Building your project..."
cmake --build build

echo "Setup complete."
