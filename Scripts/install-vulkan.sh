#!/bin/bash

requiredVulkanVersion="1.3."
installVulkanVersion="1.3.243.0"
vulkan_directory="../3rdParty/Vulkan/"

# Function to check if Vulkan SDK is installed in a directory
testVulkanSDKInstallation() {
    directory="$1"

    vulkanFiles=$(find "$directory" -type f -name "vulkan-1.lib" -o -name "vulkan-1.dll" -o -name "vulkan.hpp")

    if [ -n "$vulkanFiles" ]; then
        return 0
    else
        return 1
    fi
}

installVulkanSDK() {
    permissionGranted=false
    while [ "$permissionGranted" = false ]; do
        read -p "Would you like to install VulkanSDK $installVulkanVersion? [Y/N]: " reply
        case $reply in
            [Yy]* )
                # Download and install Vulkan SDK here (Linux-specific steps).
                echo "Downloading and installing Vulkan SDK..."
                # Insert installation steps here.
                echo "Vulkan SDK installed successfully."
                return;;
            [Nn]* )
                return;;
            * )
                echo "Please answer Y or N.";;
        esac
    done
}

searchVulkanSDK() {
    currentDirectory=$(pwd)

    while [ -n "$currentDirectory" ]; do
        if testVulkanSDKInstallation "$currentDirectory"; then
            echo "Vulkan SDK found in directory: $currentDirectory"
            return 0
        fi
        currentDirectory=$(dirname "$currentDirectory")
    done

    return 1
}

validateVulkan() {
    vulkan_sdk="$VULKAN_SDK"
    if [ -z "$vulkan_sdk" ]; then
        if testVulkanSDKInstallation "$(pwd)/VULKAN_SDK"; then
            global_vulkan_sdk_directory="$(pwd)/VULKAN_SDK"
            echo "Correct Vulkan SDK located at $global_vulkan_sdk_directory"
            return 0
        fi
        echo "You don't have the correct Vulkan SDK version! (Engine requires $requiredVulkanVersion)"
        installVulkanSDK
        return 1
    else
        if [[ ! "$vulkan_sdk" == *"$requiredVulkanVersion"* ]]; then
            echo "You don't have the correct Vulkan SDK version! (Engine requires $requiredVulkanVersion)"
            installVulkanSDK
            return 1
        fi
        global_vulkan_sdk_directory="$vulkan_sdk"
    fi

    echo "Correct Vulkan SDK located at $global_vulkan_sdk_directory"
    return 0
}

moveVulkanFiles() {
    if [ -z "$global_vulkan_sdk_directory" ]; then
        return
    fi

    # Copy the include files
    include_dir="$global_vulkan_sdk_directory/include"
    include_dest="$vulkan_directory/include"
    rsync -av --progress "$include_dir" "$include_dest"

    # Copy the library files
    library_dir="$global_vulkan_sdk_directory/lib"
    library_dest="$vulkan_directory/lib"
    rsync -av --progress "$library_dir" "$library_dest"

    # Copy the binary files
    binary_dir="$global_vulkan_sdk_directory/bin"
    binary_dest="$vulkan_directory/bin"
    rsync -av --progress "$binary_dir" "$binary_dest"

    echo "Vulkan files copied to $vulkan_directory"
}

if ! validateVulkan; then
    exit 1
fi

moveVulkanFiles
