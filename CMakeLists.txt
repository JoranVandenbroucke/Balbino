cmake_minimum_required(VERSION 3.28)
project(Balbino VERSION 0.0.1 LANGUAGES C CXX)

if (NOT DEFINED BALBINO_DIR)
    set(BALBINO_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

include(CMake/Setup.cmake)

set(TARGET_OUTPUT "Deer" CACHE STRING "Overwrite this with the exe/base project name" FORCE)
set(VULKAN_LOCATION "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan" CACHE STRING "The location of Vulkan" FORCE)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Enable Clang projects
    set(LLVM_ENABLE_PROJECTS "clang;clang-tools-extra" CACHE STRING "" FORCE)

    # Export compile commands
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

    # Use lld linker
    set(LLVM_USE_LINKER lld CACHE STRING "" FORCE)
    set(CMAKE_LINKER "D:/Program Files/LLVM/bin/lld-link.exe" CACHE STRING "" FORCE)

    # Enable atomic operations without additional libraries
    set(HAVE_CXX_ATOMICS_WITHOUT_LIB 1 CACHE BOOL "" FORCE)
    set(HAVE_CXX_ATOMICS64_WITHOUT_LIB 1 CACHE BOOL "" FORCE)

    # Enable ThinLTO
    set(LLVM_ENABLE_LTO Thin CACHE STRING "" FORCE)

    # Set C and C++ flags
    set(CMAKE_C_FLAGS "-fprofile-generate=\"D:/profile_gcc\" -femulated-tls" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS "-fprofile-generate=\"D:/profile_gcc\" -femulated-tls" CACHE STRING "" FORCE)

    # Set linker flags
    set(CMAKE_EXE_LINKER_FLAGS "-lpthread" CACHE STRING "" FORCE)
endif ()
add_subdirectory(Source/Game)
