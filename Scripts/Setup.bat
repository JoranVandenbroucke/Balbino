@echo off
set "requiredVulkanVersion=1.3"
set "installVulkanVersion=1.3.243.0"
set "vulkan_directory=./Balbino/3rdParty/VulkanSDK/"

set "vulkan_sdk="%VULKAN_SDK%""
if not defined vulkan_sdk (
    echo.
    echo You don't have the Vulkan SDK installed!
    set /p "reply=Would you like to install VulkanSDK %installVulkanVersion%? [Y/N]: "
    if /i "%reply%"=="n" exit /b
    if /i "%reply%"=="y" (
        set "vulkanInstallURL=https://sdk.lunarg.com/sdk/download/%installVulkanVersion%/windows/VulkanSDK-%installVulkanVersion%-Installer.exe"
        set "vulkanPath=%vulkan_directory%/VulkanSDK-%installVulkanVersion%-Installer.exe"
        echo Downloading %vulkanInstallURL% to %vulkanPath%
        powershell -Command "(New-Object System.Net.WebClient).DownloadFile("%vulkanInstallURL%", "%vulkanPath%")"
        echo Running Vulkan SDK installer...
        start "" "%vulkanPath%"
        echo Re-run this script after installation!
        exit /b
    )
    exit /b
) else (
    echo Located Vulkan SDK at %vulkan_sdk%
)

if not "%vulkan_sdk%"=="%vulkan_sdk:%requiredVulkanVersion%=%" (
    echo Correct Vulkan SDK located at %vulkan_sdk%
    goto continue_script
) else (
    echo You don't have the correct Vulkan SDK version! (Engine requires %requiredVulkanVersion%)
    set /p "reply=Would you like to install VulkanSDK %installVulkanVersion%? [Y/N]: "
        if /i "%reply%"=="n" exit /b
        if /i "%reply%"=="y" (
            set "vulkanInstallURL=https://sdk.lunarg.com/sdk/download/%installVulkanVersion%/windows/VulkanSDK-%installVulkanVersion%-Installer.exe"
            set "vulkanPath=%vulkan_directory%/VulkanSDK-%installVulkanVersion%-Installer.exe"
            echo Downloading %vulkanInstallURL% to %vulkanPath%
            powershell -Command "(New-Object System.Net.WebClient).DownloadFile("%vulkanInstallURL%", "%vulkanPath%")"
            echo Running Vulkan SDK installer...
            start "" "%vulkanPath%"
            echo Re-run this script after installation!
    exit /b
)
:continue_script

echo %requiredVulkanVersionFound%
echo %vulkan_sdk%

set "include_dir=%vulkan_sdk%/include"
set "include_dest=%vulkan_directory%include"
xcopy /D /I /E /Y "%include_dir%" "%include_dest%"

set "library_dir=%vulkan_sdk%/lib"
set "library_dest=%vulkan_directory%lib"
xcopy /D /I /E /Y "%library_dir%" "%library_dest%"

set "binary_dir=%vulkan_sdk%/bin"
set "binary_dest=%vulkan_directory%bin"
xcopy /D /I /E /Y "%binary_dir%" "%binary_dest%"

echo Vulkan files copied to %vulkan_directory%
