$requiredVulkanVersion = "1.3."
$installVulkanVersion = "1.3.243.0"
$vulkan_directory = "../3rdParty/Vulkan/"

function InstallVulkanSDK {
    $permissionGranted = $false
    while (-not $permissionGranted) {
        $reply = Read-Host "Would you like to install VulkanSDK $installVulkanVersion? [Y/N]"
        if ($reply -eq 'n') {
            return
        }
        $permissionGranted = ($reply -eq 'y')
    }

    $vulkanInstallURL = "https://sdk.lunarg.com/sdk/download/$installVulkanVersion/windows/VulkanSDK-$installVulkanVersion-Installer.exe"
    $vulkanPath = "$vulkan_directory\VulkanSDK-$installVulkanVersion-Installer.exe"
    Write-Host "Downloading $vulkanInstallURL to $vulkanPath"
    Invoke-WebRequest -Uri $vulkanInstallURL -OutFile $vulkanPath
    Write-Host "Running Vulkan SDK installer..."
    Start-Process $vulkanPath
    Write-Host "Re-run this script after installation!"
    exit
}

function Validate-Vulkan {
    $vulkan_sdk = $env:VULKAN_SDK
    if ($vulkan_sdk -eq $null) {
        Write-Host "You don't have the Vulkan SDK installed!" -ForegroundColor Red
        InstallVulkanSDK
        return $false
    }
    else {
        Write-Host "Located Vulkan SDK at $vulkan_sdk"
    }

    if ($vulkan_sdk -notlike "*$requiredVulkanVersion*") {
        Write-Host "You don't have the correct Vulkan SDK version! (Engine requires $requiredVulkanVersion)" -ForegroundColor Red
        InstallVulkanSDK
        return $false
    }

    $global:vulkan_sdk_directory = $vulkan_sdk
    Write-Host "Correct Vulkan SDK located at $vulkan_sdk"
    return $true
}

function Move-Vulkan-Files {
    if ($global:vulkan_sdk_directory -eq $null) {
        return
    }

    # Copy the include files
    $include_dir = Join-Path $global:vulkan_sdk_directory "include"
    $include_dest = Join-Path $vulkan_directory "include"
    if (!(Test-Path $include_dest) -or ((Get-ChildItem $include_dest | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime -lt (Get-ChildItem $include_dir | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime)) {
                Copy-Item $include_dir $include_dest -Recurse
            }

    # Copy the library files
    $library_dir = Join-Path $global:vulkan_sdk_directory "lib"
    $library_dest = Join-Path $vulkan_directory "lib"
    if (!(Test-Path $include_dest) -or ((Get-ChildItem $include_dest | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime -lt (Get-ChildItem $include_dir | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime)) {
                Copy-Item $include_dir $include_dest -Recurse
            }

    # Copy the binary files
    $binary_dir = Join-Path $global:vulkan_sdk_directory "bin"
    $binary_dest = Join-Path $vulkan_directory "bin"
    if (!(Test-Path $include_dest) -or ((Get-ChildItem $include_dest | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime -lt (Get-ChildItem $include_dir | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime)) {
                Copy-Item $include_dir $include_dest -Recurse
            }

    Write-Host "Vulkan files copied to $vulkan_directory"
}

if (-not (Validate-Vulkan)) {
    exit 1
}

Move-Vulkan-Files