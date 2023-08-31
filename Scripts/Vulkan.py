import os
import shutil

import Utils


class VulkanConfiguration:
    def __init__(self):
        self.requiredVulkanVersion = "1.3."
        self.installVulkanVersion = "1.3.243.0"
        self.vulkan_directory = "../3rdParty/Vulkan/"
        self.vulkan_sdk_directory = ""

    def __install_vulkan_sdk(self):
        permission_granted = False
        while not permission_granted:
            reply = str(input(
                "Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(
                    self.installVulkanVersion))).lower().strip()[
                    :1]
            if reply == 'n':
                return
            permission_granted = (reply == 'y')

        vulkan_install_url = f"https://sdk.lunarg.com/sdk/download/{self.installVulkanVersion}/windows/VulkanSDK-{self.installVulkanVersion}-Installer.exe"
        vulkan_path = f"{self.vulkan_directory}/VulkanSDK-{self.installVulkanVersion}-Installer.exe"
        print("Downloading {0:s} to {1:s}".format(vulkan_install_url, vulkan_path))
        Utils.download_file(vulkan_install_url, vulkan_path)
        print("Running Vulkan SDK installer...")
        os.startfile(os.path.abspath(vulkan_path))
        print("Re-run this script after installation!")
        quit()

    def validate(self):
        if not self.check_vulkan_sdk():
            print("Vulkan SDK not installed correctly.")
            return

    def check_vulkan_sdk(self):
        vulkan_sdk = os.environ.get("VULKAN_SDK")
        if vulkan_sdk is None:
            print("You don't have the Vulkan SDK installed!")
            self.__install_vulkan_sdk()
            return False
        else:
            print(f"Located Vulkan SDK at {vulkan_sdk}")

        if self.requiredVulkanVersion not in vulkan_sdk:
            print(f"You don't have the correct Vulkan SDK version! (Engine requires {self.requiredVulkanVersion})")
            self.__install_vulkan_sdk()
            return False
        self.vulkan_sdk_directory = vulkan_sdk
        print(f"Correct Vulkan SDK located at {vulkan_sdk}")
        return True

    def move(self):
        if self.vulkan_sdk_directory == "":
            return

        include_dir = os.path.join(self.vulkan_sdk_directory, "include")
        include_dest = os.path.join(self.vulkan_directory, "include")
        shutil.copytree(include_dir, include_dest)

        # Copy the library files
        library_dir = os.path.join(self.vulkan_sdk_directory, "lib")
        library_dest = os.path.join(self.vulkan_directory, "lib")
        shutil.copytree(library_dir, library_dest)

        # Copy the binary files
        binary_dir = os.path.join(self.vulkan_sdk_directory, "bin")
        binary_dest = os.path.join(self.vulkan_directory, "bin")
        shutil.copytree(binary_dir, binary_dest)

        print("Vulkan files copied to", self.vulkan_directory)
