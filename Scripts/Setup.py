import os
import pathlib
import shutil

import Vulkan

source_directory = "..\\3rdParty"
file_names = ["license.txt", "copyright.txt", "license", "copyright"]
destination_directory = pathlib.Path("../3rdParty/Copyright_and_Licence")

if __name__ == "__main__":
    vk = Vulkan.VulkanConfiguration()
    vk.validate()
    vk.move()
    for root, dirs, files in os.walk(source_directory):
        for file_name in file_names:
            for file in files:
                if file_name.lower() == file.lower():
                    source_file_path = os.path.join(root, file)
                    folder_name = os.path.basename(root)
                    destination_file_name = folder_name + "_" + file
                    destination_file_path = os.path.join(destination_directory, destination_file_name)

                    if not os.path.exists(destination_directory):
                        os.makedirs(destination_directory)

                    shutil.copyfile(source_file_path, destination_file_path)
