#!/bin/bash

requiredVulkanVersion="1.3."
installVulkanVersion="1.3.243.0"
vulkan_directory="../3rdParty/Vulkan/"

function install_vulkan_sdk() {
    while true; do
        read -p "Would you like to install VulkanSDK ${installVulkanVersion}? [Y/N]: " yn
        case $yn in
            [Yy]* )
                vulkanInstallURL="https://sdk.lunarg.com/sdk/download/${installVulkanVersion}/linux/vulkansdk-linux-x86_64-${installVulkanVersion}.tar.gz"
                vulkanPath="${vulkan_directory}/vulkansdk-linux-x86_64-${installVulkanVersion}.tar.gz"
                echo "Downloading ${vulkanInstallURL} to ${vulkanPath}"
                wget -O "${vulkanPath}" "${vulkanInstallURL}"
                echo "Extracting Vulkan SDK..."
                tar -xzf "${vulkanPath}" -C "${vulkan_directory}"
                echo "Vulkan SDK installation completed."
                break
                ;;
            [Nn]* )
                return
                ;;
            * )
                echo "Please answer yes or no."
                ;;
        esac
    done
}

function validate_vulkan_sdk() {
    vulkan_sdk="${VULKAN_SDK}"
    if [[ -z "$vulkan_sdk" ]]; then
        echo "You don't have the Vulkan SDK installed!"
        install_vulkan_sdk
        return 1
    else
        echo "Located Vulkan SDK at ${vulkan_sdk}"
    fi

    if [[ "$vulkan_sdk" != *"${requiredVulkanVersion}"* ]]; then
        echo "You don't have the correct Vulkan SDK version! (Engine requires ${requiredVulkanVersion})"
        install_vulkan_sdk
        return 1
    fi

    export VULKAN_SDK="${vulkan_sdk}"
    export PATH="${vulkan_sdk}/bin:${PATH}"
    export LD_LIBRARY_PATH="${vulkan_sdk}/lib:${LD_LIBRARY_PATH}"

    echo "Correct Vulkan SDK located at ${vulkan_sdk}"
    return 0
}

function copy_vulkan_files() {
    if [[ -z "$vulkan_sdk" ]]; then
        return
    fi

    include_dir="${vulkan_sdk}/include"
    include_dest="${vulkan_directory}/include"
    if [[ ! -d "$include_dest" ]] || ! diff -q -r "$include_dir" "$include_dest" >/dev/null; then
        echo "Copying include files..."
        rm -rf "$include_dest"
        cp -r "$include_dir" "$include_dest"
    else
        echo "Include files already up to date."
    fi

    # Copy the library files
    library_dir="${vulkan_sdk}/lib"
    library_dest="${vulkan_directory}/lib"
    if [[ ! -d "$library_dest" ]] || ! diff -q -r "$library_dir" "$library_dest" >/dev/null; then
        echo "Copying library files..."
        rm -rf "$library_dest"
        cp -r "$library_dir" "$library_dest"
    else
        echo "Library files already up to date."
    fi

    # Copy the binary files
    binary_dir="${vulkan_sdk}/bin"
    binary_dest="${vulkan_directory}/bin"
    if [[ ! -d "$binary_dest" ]] || ! diff -q -r "$binary_dir" "$binary_dest" >/dev/null; then
        echo "Copying binary files..."
        rm -rf "$binary_dest"
        cp -r "$binary_dir" "$
