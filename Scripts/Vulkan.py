import os

import Utils


class VulkanConfiguration:
    def __init__(self):
        self.requiredVulkanVersion = "1.3."
        self.installVulkanVersion = "1.3.216.0"
        self.vulkan_directory = "./Balbino/3rdParty/VulkanSDK"

    def __InstallVulkanSDK(self):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input(
                "Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(
                    self.installVulkanVersion))).lower().strip()[
                    :1]
            if reply == 'n':
                return
            permissionGranted = (reply == 'y')

        vulkanInstallURL = f"https://sdk.lunarg.com/sdk/download/{self.installVulkanVersion}/windows/VulkanSDK-{self.installVulkanVersion}-Installer.exe "
        vulkanPath = f"{self.vulkan_directory}/VulkanSDK-{self.installVulkanVersion}-Installer.exe"
        print("Downloading {0:s} to {1:s}".format(vulkanInstallURL, vulkanPath))
        Utils.DownloadFile(vulkanInstallURL, vulkanPath)
        print("Running Vulkan SDK installer...")
        os.startfile(os.path.abspath(vulkanPath))
        print("Re-run this script after installation!")
        quit()

    def Validate(self):
        if not self.CheckVulkanSDK():
            print("Vulkan SDK not installed correctly.")
            return

    def CheckVulkanSDK(self):
        vulkan_sdk = os.environ.get("VULKAN_SDK")
        if vulkan_sdk is None:
            print("\nYou don't have the Vulkan SDK installed!")
            self.__InstallVulkanSDK()
            return False
        else:
            print(f"\nLocated Vulkan SDK at {vulkan_sdk}")

        if self.requiredVulkanVersion not in vulkan_sdk:
            print(f"You don't have the correct Vulkan SDK version! (Engine requires {self.requiredVulkanVersion})")
            self.__InstallVulkanSDK()
            return False

        print(f"Correct Vulkan SDK located at {vulkan_sdk}")
        return True
