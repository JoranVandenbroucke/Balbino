cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)

if (NOT DEFINED BALBINO_DIR)
    set(BALBINO_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

include(CMake/Setup.cmake)

project(Balbino
        VERSION "0.0.1.0"
        LANGUAGES C CXX
)

set(TARGET_OUTPUT "Deer" CACHE STRING "Overwrite this with the exe/base project name" FORCE)
set(SDL_LOCATION "${CMAKE_SOURCE_DIR}/3rdParty/SDL" CACHE STRING "The location of SDL3" FORCE)
set(VULKAN_LOCATION "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan" CACHE STRING "The location of Vulkan" FORCE)

add_compile_definitions(
        $<$<PLATFORM_ID:Linux>:-DBALBINO_LINUX>
        $<$<PLATFORM_ID:Windows>:-DBALBINO_WINDOWS>
        $<$<PLATFORM_ID:Darwin>:-DBALBINO_MACOS>
)
add_subdirectory(Source/Game)
