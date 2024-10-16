$requiredVulkanVersion = "1.3*"
$installVulkanVersion = "1.3.275.0"
$vulkan_directory = Join-Path $PSScriptRoot "../3rdParty/Vulkan/"

# Function to check if Vulkan SDK is installed in a directory
function Test-VulkanSDKInstallation
{
    param (
        [string]$directory
    )

    Write-Host $directory

    $vulkanFiles = Get-ChildItem -Path $directory -File -Recurse | Select-Object -ExpandProperty Name
    $requiredFiles = @("vulkan-1.lib", "vulkan-1.dll", "vulkan.hpp")

    foreach ($file in $requiredFiles)
    {
        if ($vulkanFiles -contains $file)
        {
            return $true
        }
    }
    return $false
}

$loop_count = 0
function InstallVulkanSDK
{
    if ($loop_count -eq 1)
    {
        return $false
    }
    $loop_count = 1
    $permissionGranted = $false
    while (-not$permissionGranted)
    {
        $reply = Read-Host "Would you like to install VulkanSDK $installVulkanVersion? [Y/N]"
        if ($reply -eq 'n')
        {
            return $false
        }
        $permissionGranted = ($reply -eq 'y')
    }

    $vulkanInstallURL = "https://sdk.lunarg.com/sdk/download/$installVulkanVersion/windows/VulkanSDK-$installVulkanVersion-Installer.exe"
    $vulkanPath = "$vulkan_directory\VulkanSDK-$installVulkanVersion-Installer.exe"
    New-Item -Path $vulkan_directory -ItemType "directory"
    Write-Host "Downloading $vulkanInstallURL to $vulkanPath"
    Invoke-WebRequest -Uri $vulkanInstallURL -OutFile $vulkanPath
    Write-Host "Running Vulkan SDK installer..."
    Start-Process $vulkanPath
    Write-Host "Re-run this script after installation!"
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")

    return Validate-Vulkan
}

function Validate-Vulkan
{
    $vulkan_sdk = $env:VULKAN_SDK
    $testDirectory = Join-Path $PSScriptRoot "VULKAN_SDK"
    Write-Host $vulkan_sdk
    if ($null -eq $vulkan_sdk)
    {
        Write-Host "vulkan not found via Environment Variables."
        if (Test-VulkanSDKInstallation $testDirectory)
        {
            $global:vulkan_sdk_directory = $testDirectory
            Write-Host "Correct Vulkan SDK located at $testDirectory"
            return $true
        }
        else
        {
            Write-Host "$testDirectory does not exist!"
        }

        Write-Host "You don't have Vulkan SDK!" -ForegroundColor Red
        return InstallVulkanSDK
    }
    else
    {
        Write-Host "Found vulkan at $vulkan_sdk"

        $vulkaninfo_exe = Join-Path -Path $vulkan_sdk -ChildPath "Bin\vulkaninfo.exe"
        if (-not(Test-Path -Path $vulkaninfo_exe))
        {
            $vulkaninfo_exe = Join-Path -Path $vulkan_sdk -ChildPath "Bin32\vulkaninfo.exe"
        }
        if (-not(Test-Path -Path $vulkaninfo_exe))
        {
            $vulkaninfo_exe = Join-Path -Path $vulkan_sdk -ChildPath "Bin64\vulkaninfo.exe"
        }

        if (Test-Path -Path $vulkaninfo_exe)
        {
            $vulkan_version = & $vulkaninfo_exe --summary |`
                        Select-String 'Vulkan API Version' |`
                        % { $_.ToString().Split()[-1].Split('.')[0, 1] -join '.' }
            Write-Output ("Installed Vulkan Version: " + $vulkan_version)
            if ($vulkan_version -like $requiredVulkanVersion)
            {
                Write-Output "Vulkan version is correct."
                if (-not($vulkan_sdk.Contains($requiredVulkanVersion)))
                {
                    Write-Host "You don't have the correct Vulkan SDK version! (Engine requires $requiredVulkanVersion)" -ForegroundColor Red
                    InstallVulkanSDK
                    return $false
                }
            }
            else
            {
                Write-Output "Vulkan version is incorrect. Engine requires version starting with 1.3"
            }
        }
        else
        {
            Write-Output "Cannot find vulkaninfo.exe in the Vulkan SDK directory"
        }
        $global:vulkan_sdk_directory = $vulkan_sdk
    }

    Write-Host "Correct Vulkan SDK located at $vulkan_sdk"
    return $true
}

function Move-Vulkan-Files
{
    if ($null -eq $global:vulkan_sdk_directory)
    {
        return
    }

    # Copy the include files
    $include_dir = Join-Path $global:vulkan_sdk_directory "include"
    $include_dest = Join-Path $vulkan_directory "include"
    Write-Host "Copying $include_dir to $include_dest"
    if (!(Test-Path $include_dest) -or ((Get-ChildItem $include_dest | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime -lt (Get-ChildItem $include_dir | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime))
    {
        Copy-Item $include_dir $include_dest -Recurse
    }

    # Copy the library files
    $library_dir = Join-Path $global:vulkan_sdk_directory "lib"
    $library_dest = Join-Path $vulkan_directory "lib"
    Write-Host "Copying $library_dir to $library_dest"
    if (!(Test-Path $library_dest) -or ((Get-ChildItem $library_dest | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime -lt (Get-ChildItem $library_dir | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime))
    {
        Copy-Item $library_dir $library_dest -Recurse
    }

    # Copy the binary files
    $binary_dir = Join-Path $global:vulkan_sdk_directory "bin"
    $binary_dest = Join-Path $vulkan_directory "bin"
    Write-Host "Copying $binary_dir to $binary_dest"
    if (!(Test-Path $binary_dest) -or ((Get-ChildItem $binary_dest | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime -lt (Get-ChildItem $binary_dir | Sort-Object LastWriteTime | Select-Object -Last 1).LastWriteTime))
    {
        Copy-Item $binary_dir $binary_dest -Recurse
    }

    Write-Host "Vulkan files copied to $vulkan_directory"
}

if (-not(Validate-Vulkan))
{
    exit 1
}

Move-Vulkan-Files
