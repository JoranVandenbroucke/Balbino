# Call the install-vulkan.ps1 script
Write-Host "Running install-vulkan.ps1..."
Invoke-Expression -Command "Scripts/install-vulkan.ps1"

# Perform CMake submodule initialization
Write-Host "Initializing CMake submodules..."
cmake -B build -S . -DBUILD_TESTS=OFF

# Build your project
#Write-Host "Building your project..."
#cmake --build build

Write-Host "Setup complete."
