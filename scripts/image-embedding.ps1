Add-Type -AssemblyName System.Drawing

function Convert-ImageToHex($imagePath) {
    $image = [System.Drawing.Image]::FromFile($imagePath)
    $imageWidth = $image.Width
    $imageHeight = $image.Height
    $data = [System.IO.File]::ReadAllBytes($imagePath)
    $imageSize = $data.Length
    $imageDataHex = $data | ForEach-Object { '0x{0:X2}' -f $_ }

    # Convert the comma-separated string to an array of values
    $imageDataArray = $imageDataHex -split ', ' | ForEach-Object { $_.Replace('0x', '') } | ForEach-Object { '0x' + $_ }

    # Join the array elements with commas and wrap them in curly braces
    $imageDataArrayString = '{' + ($imageDataArray -join ', ') + '}'

    return $imageDataArrayString, $imageWidth, $imageHeight, $imageSize
}

$embedded = Join-Path $PSScriptRoot "..\Source\Engine\Embedded\Images.hpp"
# Clear the file at the beginning
if (Test-Path $embedded) {
    Clear-Content $embedded
}
Add-Content $embedded "#pragma once"

# Loop through all images in the directory and append them to the same file
Get-ChildItem -Path (Join-Path $PSScriptRoot ..\Embedded\Images\*) -Include *.png | ForEach-Object {
    $path = $_.FullName
    $hexData, $width, $height, $size = Convert-ImageToHex $path

    # Get the name of the image without the extension
    $imageName = $_.BaseName

    # Append the image data to the file
    Add-Content $embedded "const uint8_t g_$imageName[] = $hexData;"
    Add-Content $embedded "constexpr uint32_t g_${imageName}_width = $width;"
    Add-Content $embedded "constexpr uint32_t g_${imageName}_height = $height;"
    Add-Content $embedded "constexpr uint32_t g_${imageName}_size = $size;"
}
