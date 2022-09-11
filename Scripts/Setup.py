# from https://github.com/TheCherno/Hazel/blob/master/scripts/SetupVulkan.py
import os
import shutil
import subprocess
from pathlib import Path

import Utils


class VulkanConfiguration:
    requiredVulkanVersion = "1.3."
    installVulkanVersion = "1.3.216.0"
    vulkan_directory = "./Balbino/3rdParty/VulkanSDK"

    @classmethod
    def Validate(cls):
        if not cls.CheckVulkanSDK():
            print("Vulkan SDK not installed correctly.")
            return

        if not cls.CheckVulkanSDKDebugLibs():
            print("\nNo Vulkan SDK debug libs found. Install Vulkan SDK with debug libs.")
            print("(see docs.hazelengine.com/GettingStarted for more info).")
            print("Debug configuration disabled.")

    @classmethod
    def CheckVulkanSDK(cls):
        vulkan_sdk = os.environ.get("VULKAN_SDK")
        if vulkan_sdk is None:
            print("\nYou don't have the Vulkan SDK installed!")
            cls.__InstallVulkanSDK()
            return False
        else:
            print(f"\nLocated Vulkan SDK at {vulkan_sdk}")

        if cls.requiredVulkanVersion not in vulkan_sdk:
            print(f"You don't have the correct Vulkan SDK version! (Engine requires {cls.requiredVulkanVersion})")
            cls.__InstallVulkanSDK()
            return False

        print(f"Correct Vulkan SDK located at {vulkan_sdk}")
        return True

    @classmethod
    def __InstallVulkanSDK(cls):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input(
                "Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(cls.installVulkanVersion))).lower().strip()[
                    :1]
            if reply == 'n':
                return
            permissionGranted = (reply == 'y')

        vulkanInstallURL = f"https://sdk.lunarg.com/sdk/download/{cls.installVulkanVersion}/windows/VulkanSDK-{cls.installVulkanVersion}-Installer.exe"
        vulkanPath = f"{cls.vulkan_directory}/VulkanSDK-{cls.installVulkanVersion}-Installer.exe"
        print("Downloading {0:s} to {1:s}".format(vulkanInstallURL, vulkanPath))
        Utils.DownloadFile(vulkanInstallURL, vulkanPath)
        print("Running Vulkan SDK installer...")
        os.startfile(os.path.abspath(vulkanPath))
        print("Re-run this script after installation!")
        quit()

    @classmethod
    def CheckVulkanSDKDebugLibs(cls):
        vulkanSDK = os.environ.get("VULKAN_SDK")
        shadercdLib = Path(f"{vulkanSDK}/Lib/shaderc_sharedd.lib")

        return shadercdLib.exists()


if __name__ == "__main__":
    VulkanConfiguration.Validate()
    cwd = os.getcwd()
    print(cwd + r'\..\3rdParty\assimp\BUILDBINARIES_EXAMPLE.bat')
    os.chdir(cwd + r'\..\3rdParty\assimp')
    subprocess.call([r'BUILDBINARIES_EXAMPLE.bat'])
    os.chdir(cwd + r'\..\3rdParty\assimp')
    shutil.copyfile(r'build/x64/include/assimp/config.h', r'include/assimp/config.h')
    # os.chdir(cwd + r'\..\3rdParty\PhysX\physx')
    # subprocess.call([r'generate_projects.bat'])
