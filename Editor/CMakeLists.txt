cmake_minimum_required(VERSION 3.22.2)
set(CMAKE_CONFIGURATION_TYPES Debug;DebugEditor;Release;ReleaseEditor;)

# Set Project Name
project(BalEditor)

add_subdirectory(../Common Common)
add_subdirectory(../Renderer/GraphicsVulkan BalVulkan)
add_subdirectory(../Renderer/Imgui ImGui)

# get source files
file(GLOB sourceE
        scr/*.cpp scr/*.h
        scr/Tools/FilesSystem/*.cpp scr/Tools/FilesSystem/*.h
        scr/Tools/FilesSystem/Inporter/*.cpp scr/Tools/FilesSystem/Inporter/*.h scr/Tools/FilesSystem/Inporter/*.c
        scr/Windows/*.cpp scr/Windows/*.h
        scr/Windows/Nodes/*.cpp scr/Windows/Nodes/*.h
        )

# project setting
add_library(BalEditor ${sourceE})
set_property(TARGET BalEditor PROPERTY CXX_STANDARD 23)
#set_property(TARGET BalEditor PROPERTY C_STANDARD 17)
set_property(TARGET BalEditor PROPERTY CXX_STANDARD_REQUIRED ON)
#set_property(TARGET BalEditor PROPERTY C_STANDARD_REQUIRED ON)

if (MSVC)
    target_compile_options(BalEditor PRIVATE
            $<$<CONFIG:Debug>:
            /Od;
            /ZI;
            /Ob0;
            /RTC1;
            /MDd;
            >
            $<$<CONFIG:DebugEditor>:
            /Od;
            /ZI;
            /Ob0;
            /RTC1;
            /MDd;
            >
            $<$<CONFIG:Release>:
            /Ox
            >
            $<$<CONFIG:ReleaseEditor>:
            /Ox
            >
            /MP;
            /W4;
            /WX;
            /std:c++latest;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            )
    target_compile_definitions(BalEditor
            PRIVATE
            "_UNICODE;"
            "UNICODE;"
            "$<$<CONFIG:Debug>:"
            "_DEBUG;"
            ">"
            "$<$<CONFIG:DebugEditor>:"
            "_DEBUG;"
            ">"
            "$<$<CONFIG:Release>:"
            "NDEBUG;"
            ">"
            "$<$<CONFIG:ReleaseEditor>:"
            "NDEBUG;"
            ">"
            "BL_PLATFORM_WINDOWS"
            )
    target_link_options(BalEditor PRIVATE
            /INCREMENTAL;
            $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE;
            >
            $<$<CONFIG:DebugEditor>:
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE;
            >
            $<$<CONFIG:Release>:
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS;
            >
            $<$<CONFIG:ReleaseEditor>:
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS;
            >
            )
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

target_include_directories(BalEditor
        PRIVATE
        ../3rdParty/SDL2/include
        ../3rdParty/SDL2_image/include
        ../3rdParty/SDL2_mixer/include
        ../3rdParty/SDL2_ttf/include
        ../3rdParty/EnTT/single_include
        ../3rdParty/ASSIMP/include
        ../3rdParty/ASSIMP/out/build/x64-Debug/include
        ../3rdParty/ASSIMP/out/build/windows-default/include
        ../3rdParty/GLM
        ../3rdParty/mINI/src/mini
        ../3rdParty/GLI
        ../3rdParty/Vulkan/Include
        ../Renderer/GraphicsVulkan
        ../Renderer/Imgui
        ../Common
        )