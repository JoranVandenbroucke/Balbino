set(PROJECT_NAME Balbino)

# get source files
file(GLOB_RECURSE source
        *.cpp *.h
        **/*.cpp **/*.h
        )

# project setting
add_library(${PROJECT_NAME} ${source})
target_precompile_headers(${PROJECT_NAME}
        PUBLIC
        [["algorithm"]]
        [["array"]]
        [["cmath"]]
        [["cstdint"]]
        [["format"]]
        [["iostream"]]
        [["unordered_map"]]
        [["vector"]]
        [["SDL.h"]]
        [["SDL_image.h"]]
        [["SDL_mixer.h"]]
        [["SDL_ttf.h"]]
        )

set(ROOT_NAMESPACE Balbino)

#include
target_include_directories(${PROJECT_NAME} PRIVATE
        ../3rdParty/SDL2/include
        ../3rdParty/SDL2_image/include
        ../3rdParty/SDL2_mixer/include
        ../3rdParty/SDL2_ttf/include
        ../3rdParty/EnTT/single_include
        ../3rdParty/GLM
        ../3rdParty/mINI/src/mini
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

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "BL_PLATFORM_WINDOWS;"
        "BL_BUILD_LIB;"
        "$<$<CONFIG:Debug>:"
        "_DEBUG"
        ">"
        "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
        "BALBINO_EDITOR;"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
        "BALBINO_EDITOR;"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE;"
        "_LIB"
        )

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /Zi;
            /Ob0;
            /Od;
            /RTC1;
            /MDd;
            >
            $<$<CONFIG:DebugEditor>:
            /Zi;
            /Ob0;
            /Od;
            /RTC1;
            /MDd;
            >
            $<$<CONFIG:Release>:
            /Ox;
            /Ob2;
            /Oi;
            /Gy
            >
            $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Ob2;
            /Oi;
            /Gy
            >
            /std:c++20;
            /permissive-;
            /sdl;
            /W4;
            /WX;
            /MP;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
            )
    target_link_options(${PROJECT_NAME} PRIVATE
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
            /OPT:REF;
            /OPT:ICF;
            /SUBSYSTEM:WINDOWS;
            >
            $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF;
            /SUBSYSTEM:WINDOWS;
            >
            )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall;
            -Werror;
            -std=c++2b;
            )
endif ()

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
        ">"

        "$<$<CONFIG:DebugEditor>:"
        "assimp-vc143-mtd;"
        "spirv-cross-cored;"
        "spirv-cross-glsld;"
        "spirv-cross-reflectd;"
        ">"

        "$<$<CONFIG:Release>:"
        "spirv-cross-core;"
        "spirv-cross-glsl;"
        "spirv-cross-reflect;"
        ">"

        "$<$<CONFIG:ReleaseEditor>:"
        "assimp-vc143-mt;"
        "spirv-cross-core;"
        "spirv-cross-glsl;"
        "spirv-cross-reflect;"
        ">"
        )

# subdirectories
add_dependencies(${PROJECT_NAME}
        Editor
        Common
        )

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
        Editor;
        Common;
        "${libs}"
        )

target_link_directories(${PROJECT_NAME} PRIVATE
        "../3rdParty/SDL2/lib/x64/;"
        "../3rdParty/SDL2_image/lib/x64/;"
        "../3rdParty/SDL2_mixer/lib/x64/;"
        "../3rdParty/SDL2_ttf/lib/x64/;"
        "../3rdParty/SPIRV-Cross/lib/Release;"
        "../3rdParty/SPIRV-Cross/lib/Debug;"
        "../3rdParty/Vulkan/lib/;"
        "../3rdParty/ASSIMP/build/x64/lib/Debug;"
        "../3rdParty/ASSIMP/build/x64/lib/Release"
        )
message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")
#add_custom_command(
#        TARGET Balbino POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Balbino.dll ${CMAKE_CURRENT_BINARY_DIR}/../Game/Balbino.dll
#)