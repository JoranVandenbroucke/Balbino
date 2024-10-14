# Call the install-vulkan.ps1 script
Write-Host "Running install-vulkan.ps1..."
Invoke-Expression -Command "Scripts\install-vulkan.ps1"

Write-Host "Running image-embedding.ps1..."
Invoke-Expression -Command "Scripts\image-embedding.ps1"
Write-Host "Running spirv-embedding.py..."
Invoke-Expression -Command "Scripts\spirv-embedding.ps1"

# Write-Host "Initializing CMake submodules..."
# cmake -B build -S .

Write-Host "Setup complete."
