function ImageToCppArray {
    param (
        [string]$ImagePath,
        [string]$ArrayName
    )

    # Read binary data from the image
    $ImageData = Get-Content -Path $ImagePath -Encoding Byte

    # Convert binary data to a C array
    $CppArray = $ImageData | ForEach-Object { "0x{0:X2}" -f $_ }

    # Generate the C++ code
    $CppCode = @"
const uint8_t $ArrayName[] = {{
$($CppArray -join ', ')
}};
const uint32_t $($ArrayName)_size = $($ImageData.Count);
"@

    return $CppCode
}

function WriteCppFile {
    param (
        [string]$CppCode,
        [string]$OutputFile
    )

    # Write the C++ code to a file
    Set-Content -Path $OutputFile -Value $CppCode

    Write-Host "C++ code generated and saved to $OutputFile"
}

$imagePaths = @(
    @{ Path = "../images/window_splach_screen_icon.png"; ArrayName = "g_windowSplashScreen" },
    @{ Path = "../images/window_icon.png"; ArrayName = "g_windowIcon" }
)

foreach ($imageInfo in $imagePaths) {
    $cppCode = ImageToCppArray -ImagePath $imageInfo.Path -ArrayName $imageInfo.ArrayName
    WriteCppFile -CppCode $cppCode -OutputFile "$($imageInfo.ArrayName).h"
}
