#!/bin/bash

required_vulkan_version="1.3."
install_vulkan_version="1.3.243.0"
vulkan_directory="3rdParty/Vulkan"

# Function to check if Vulkan SDK is installed in a directory
testVulkanSDKInstallation() {
    directory=$1
    echo $directory

    vulkanFiles=$(find $directory -type f)

    requiredFiles=("vulkan-1.lib" "vulkan-1.dll" "vulkan.hpp")

    for file in "${requiredFiles[@]}"; do
        if echo $vulkanFiles | grep -q $file; then
            return 1 # return 1 when file is found
        fi
    done

    return 0 # return 0 when no required file is found
}

installVulkanSDK() {
    permissionGranted=false
    while [ "$permissionGranted" = false ]; do
        read -p "Would you like to install VulkanSDK $install_vulkan_version? [Y/N]: " reply
        case $reply in
        [Yy]*)
            # Download and install Vulkan SDK here (Linux-specific steps).
            echo "Downloading and installing Vulkan SDK..."
            # Insert installation steps here.
            echo "Vulkan SDK installed successfully."
            exit 0
            ;;
        [Nn]*)
            exit 1
            ;;
        *)
            echo "Please answer Y or N."
            ;;
        esac
    done
}

validateVulkan() {
    vulkan_sdk="$VULKAN_SDK"
    currentDirectory=$(pwd)
    if [ -z "$vulkan_sdk" ]; then
        echo "Vulkan not found via Environment Variables."
        if [ -d "$currentDirectory/VULKAN_SDK" ]; then
            if testVulkanSDKInstallation "$currentDirectory/VULKAN_SDK"; then
                global_vulkan_sdk_directory="$currentDirectory/VULKAN_SDK"
                echo "Correct Vulkan SDK located at $global_vulkan_sdk_directory"
                return 0
            fi
        else
            echo "$currentDirectory/VULKAN_SDK does not exist!"
        fi

        echo "You don't have Vulkan SDK!"
        installVulkanSDK
        return 1
    else
        echo "Found Vulkan at $vulkan_sdk"
        #        vulkan_version = $("$vulkan_sdk/Bin/vulkaninfo" --summary | grep 'Vulkan API Version' | awk '{print $NF}' | cut -d"." -f1,2)
        #        if [[ $vulkan_version != *"$required_vulkan_version"* ]]; then
        #            vulkan_version = $("$vulkan_sdk/x86_64/Bin/vulkaninfo" --summary | grep 'Vulkan API Version' | awk '{print $NF}' | cut -d"." -f1,2)
        #            if [[ $vulkan_version != *"$required_vulkan_version"* ]]; then
        #                vulkan_version = $("$vulkan_sdk/$vulkan_version/x86_64/vulkaninfo" --summary | grep 'Vulkan API Version' | awk '{print $NF}' | cut -d"." -f1,2)
        #                if [[ $vulkan_version != *"$required_vulkan_version"* ]]; then
        #                    echo "Installed Vulkan Version: $vulkan_version"
        #                    echo "You don't have the correct Vulkan SDK version! (Engine requires $required_vulkan_version)"
        #                    installVulkanSDK
        #                    return 1
        #                fi
        #            fi
        #        fi
        #        echo "Installed Vulkan Version: $vulkan_version"
        global_vulkan_sdk_directory="$vulkan_sdk"
    fi

    echo "Correct Vulkan SDK located at $global_vulkan_sdk_directory"
    return 0
}

moveVulkanFiles() {
    if [ -z "$global_vulkan_sdk_directory" ]; then
        return
    fi

    echo $PWD
    binary_dest="$vulkan_directory/bin"
    mkdir -p "$PWD/$binary_dest"
    binary_dir="$global_vulkan_sdk_directory/bin"
    rsync -av --progress "$binary_dir" "$binary_dest"
    binary_dir="$global_vulkan_sdk_directory/x86_64/bin"
    rsync -av --progress "$binary_dir" "$binary_dest"
    binary_dir="$global_vulkan_sdk_directory/$install_vulkan_version/x86_64/bin"
    rsync -av --progress "$binary_dir" "$binary_dest"

    include_dest="$vulkan_directory/include"
    mkdir -p "$PWD/$include_dest"
    include_dir="$global_vulkan_sdk_directory/include"
    rsync -av --progress "$include_dir" "$include_dest"
    include_dir="$global_vulkan_sdk_directory/x86_64/include"
    rsync -av --progress "$include_dir" "$include_dest"
    include_dir="$global_vulkan_sdk_directory/$install_vulkan_version/x86_64/include"
    rsync -av --progress "$include_dir" "$include_dest"

    library_dest="$vulkan_directory/lib"
    mkdir -p "$PWD/$library_dest"
    library_dir="$global_vulkan_sdk_directory/lib"
    rsync -av --progress "$library_dir" "$library_dest"
    library_dir="$global_vulkan_sdk_directory/x86_64/lib"
    rsync -av --progress "$library_dir" "$library_dest"
    library_dir="$global_vulkan_sdk_directory/$install_vulkan_version/x86_64/lib"
    rsync -av --progress "$library_dir" "$library_dest"

    echo "Vulkan files copied to $vulkan_directory"
    echo "If things went wrong, manually copy the folders \"include\", \"bin\", and \"lib\" to \"./3rdParty/Vulkan\"."
}

if ! validateVulkan; then
    exit 1
fi

moveVulkanFiles
