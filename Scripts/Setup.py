# from https://github.com/TheCherno/Hazel/blob/master/scripts/SetupVulkan.py
import os
import shutil
import subprocess
import sys

import Vulkan


def HandleError(err):
    if err.returncode != 0:
        print(f'something went wrong while running {err.args}.\nError code was {err.returncode}')
        quit()


def GenerateCMake(setup_extra: list[str] = None, debug_extra: list[str] = None, release_extra: list[str] = None):
    cmake_arg = ['-A', 'x64', '-B', './build/x64']
    debug_arg = ['--config', 'debug']
    relea_arg = ['--config', 'release']
    if setup_extra:
        cmake_arg = cmake_arg + setup_extra
    if debug_extra:
        debug_arg = debug_arg + debug_extra
    if release_extra:
        relea_arg = relea_arg + release_extra

    # cmake_cmd = ['cmake.exe', '-G', 'Visual Studio 17 2022', '-A', 'x64', '-S', '.', '-B', './build/x64']
    HandleError(subprocess.run(['cmake.exe', '-S', '.', '-G', 'Visual Studio 17 2022'] + cmake_arg, stderr=subprocess.STDOUT, shell=shell))
    HandleError(subprocess.run(['cmake.exe', '--build', './build/x64'] + debug_arg, stderr=subprocess.STDOUT, shell=shell))
    HandleError(subprocess.run(['cmake.exe', '--build', './build/x64'] + relea_arg, stderr=subprocess.STDOUT, shell=shell))


if __name__ == "__main__":
    vk = Vulkan.VulkanConfiguration()
    vk.Validate()

    if sys.platform.startswith('win'):
        shell = True
    else:
        shell = False

    cwd = os.getcwd()
    os.chdir(cwd + r'\..\3rdParty\assimp')
    print(os.getcwd())

    GenerateCMake()

    print(os.getcwd())
    shutil.copyfile(r'build/x64/include/assimp/config.h', r'include/assimp/config.h')

    cwd = os.getcwd()
    os.chdir(cwd + r'\..\GLM')
    print(os.getcwd())

    GenerateCMake()

    cwd = os.getcwd()
    os.chdir(cwd + r'\..\GLI')
    print(os.getcwd())

    GenerateCMake(['-DGLI_TEST_ENABLE=OFF'])
