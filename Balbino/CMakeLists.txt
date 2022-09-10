cmake_minimum_required(VERSION 3.22.2)
set(CMAKE_CONFIGURATION_TYPES Debug;DebugEditor;Release;ReleaseEditor;)

# Set Project Name
project(Balbino)

# subdirectories
add_subdirectory(../Editor BalEditor)

# get source files
file(GLOB sourceB
        *.cpp *.h
        scr/*.cpp scr/*.h
        scr/Audio/*.cpp scr/Audio/*.h
        scr/Input/*.cpp scr/Input/*.h
        scr/Managers/*.cpp scr/Managers/*.h
        scr/Scene/*.cpp scr/Scene/*.h
        scr/Renderer/*.cpp scr/Renderer/*.h
        )

# project setting
add_library(Balbino SHARED ${sourceB})
set_property(TARGET Balbino PROPERTY CXX_STANDARD 20)
set_property(TARGET Balbino PROPERTY C_STANDARD 17)
set_property(TARGET Balbino PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET Balbino PROPERTY C_STANDARD_REQUIRED ON)

set_target_properties(Balbino PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR/BIN/${PROJECT_NAME}}/"
        LIBRARY_OUTPUT_DIRECTORY_DEBUGEDITOR "${CMAKE_SOURCE_DIR/BIN/${PROJECT_NAME}}/"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR/BIN/${PROJECT_NAME}}/"
        LIBRARY_OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR/BIN/${PROJECT_NAME}}/"
        )

if (MSVC)
    target_compile_options(Balbino PRIVATE
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
            /Ox;
            >
            $<$<CONFIG:ReleaseEditor>:
            /Ox;
            >
            /MP;
            /W4;
            /WX;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
            )
    target_link_options(Balbino PRIVATE
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
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

target_precompile_headers(Balbino
        PRIVATE [["algorithm"]]
        PRIVATE [["chrono"]]
        PRIVATE [["cstdio"]]
        PRIVATE [["cstdint"]]
        PRIVATE [["filesystem"]]
        PRIVATE [["fstream"]]
        PRIVATE [["iostream"]]
        PRIVATE [["memory"]]
        PRIVATE [["regex"]]
        PRIVATE [["sstream"]]
        PRIVATE [["string"]]
        PRIVATE [["unordered_map"]]
        PRIVATE [["vector"]]
        PRIVATE [["SDL.h"]]
        PRIVATE [["SDL_image.h"]]
        PRIVATE [["SDL_mixer.h"]]
        PRIVATE [["SDL_ttf.h"]]
        )

install(
        TARGETS Balbino
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

target_compile_definitions(Balbino
        PRIVATE
        "BL_PLATFORM_WINDOWS;"
        "BL_BUILD_DLL;"
        "_WINDLL;"
        "_UNICODE;"
        "UNICODE;"
        $<$<CONFIG:DebugEditor>:BALBINO_EDITOR>
        $<$<CONFIG:ReleaseEditor>:BALBINO_EDITOR>
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
        )

#include
target_include_directories(Balbino
        PRIVATE
        ../3rdParty/SDL2/include
        ../3rdParty/SDL2_image/include
        ../3rdParty/SDL2_mixer/include
        ../3rdParty/SDL2_ttf/include
        ../3rdParty/EnTT/single_include
        ../3rdParty/GLM
        ../3rdParty/mINI/src/mini
        ../3rdParty/VLD/include
        ../3rdParty/Vulkan/Include
        ../3rdParty/ASSIMP/include
        ../Renderer/GraphicsVulkan
        ../Common
        $<$<CONFIG:DebugEditor>:
        ../Editor
        >
        $<$<CONFIG:ReleaseEditor>:
        ../Editor
        >
        )

target_link_directories(Balbino PRIVATE
        ../3rdParty/SDL2/lib/x64/
        ../3rdParty/SDL2_image/lib/x64/
        ../3rdParty/SDL2_mixer/lib/x64/
        ../3rdParty/SDL2_ttf/lib/x64/
        ../3rdParty/SPIRV-Cross/lib/Release
        ../3rdParty/SPIRV-Cross/lib/Debug
        ../3rdParty/Vulkan/lib/
        ../3rdParty/VLD/lib/Win64
        ../3rdParty/ASSIMP/build/x64/lib/Debug
        ../3rdParty/ASSIMP/build/x64/lib/Release
        )

set(libs
        "SDL2;"
        "SDL2main;"
        "SDL2_image;"
        "SDL2_mixer;"
        "SDL2_ttf;"
        "vulkan-1;"
        "shaderc_shared;"

        "$<$<CONFIG:Debug>:"
        "spirv-cross-cored;"
        "spirv-cross-glsld;"
        "spirv-cross-reflectd;"
        "vld"
        ">"

        "$<$<CONFIG:DebugEditor>:"
        "BalEditor;"
        "ImGui;"
        "assimp-vc142-mtd;"
        "spirv-cross-cored;"
        "spirv-cross-glsld;"
        "spirv-cross-reflectd;"
        "vld"
        ">"

        "$<$<CONFIG:Release>:"
        "spirv-cross-core;"
        "spirv-cross-glsl;"
        "spirv-cross-reflect;"
        ">"

        "$<$<CONFIG:ReleaseEditor>:"
        "BalEditor;"
        "ImGui;"
        "assimp-vc142-mt;"
        "spirv-cross-core;"
        "spirv-cross-glsl;"
        "spirv-cross-reflect;"
        ">"
        )
target_link_libraries(Balbino
        PRIVATE
        BalVulkan
        "${libs}"
        )
add_custom_command(
        TARGET Balbino POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Balbino.dll ${CMAKE_CURRENT_BINARY_DIR}/../Balbino.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../../3rdParty/Vulkan/Lib/shaderc_shared.dll ${CMAKE_CURRENT_BINARY_DIR}/../shaderc_shared.dll
)