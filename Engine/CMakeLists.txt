set(PROJECT_NAME Balbino)

# get source files
file(GLOB_RECURSE source
        Core/*.cpp Core/*.h
        )

# project setting
add_library(${PROJECT_NAME} STATIC Balbino.h ${source})

set( SDL_TEST OFF CACHE BOOL "" FORCE )
set( BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE )
add_subdirectory( ../3rdParty/SDL SDL EXCLUDE_FROM_ALL)

set( BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE )
add_subdirectory( ../3rdParty/SDL_image SDL_image EXCLUDE_FROM_ALL )
set( BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE )
add_subdirectory( ../3rdParty/SDL_mixer SDL_mixer EXCLUDE_FROM_ALL )
set( BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE )
add_subdirectory( ../3rdParty/SDL_ttf SDL_ttf EXCLUDE_FROM_ALL )

add_dependencies(${PROJECT_NAME}
        Common
        Renderer
        Editor
        GraphicsVulkan
        )

target_precompile_headers(${PROJECT_NAME} PRIVATE
        [["algorithm"]]
        [["array"]]
        [["cmath"]]
        [["cstdint"]]
        [["format"]]
        [["iostream"]]
        [["unordered_map"]]
        [["vector"]]
        [["glm/glm.hpp"]]
        )

set(ROOT_NAMESPACE Balbino)

#include
target_include_directories(${PROJECT_NAME} PRIVATE
        Common
        Renderer
        Renderer/API/Vulkan
        "$<$<CONFIG:DebugEditor>:"
        Editor
        >
        "$<$<CONFIG:ReleaseEditor>:"
        Editor
        >
        ../3rdParty/EnTT/single_include
        ../3rdParty/GLM/
        ../3rdParty/mINI/src
        ../3rdParty/SDL/include
        ../3rdParty/Vulkan/include
        )

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "BL_PLATFORM_WINDOWS;"
        "BL_BUILD_LIB;"
        "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_CONSOLE;"
        ">"
        "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_CONSOLE;"
        "BALBINO_EDITOR;"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG;"
        ">"
        "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG;"
        "BALBINO_EDITOR;"
        ">"
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
            $<$<CONFIG:Debug>:
            /INCREMENTAL;
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE;
            >
            $<$<CONFIG:DebugEditor>:
            /INCREMENTAL;
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE;
            >
            $<$<CONFIG:Release>:
            /INCREMENTAL:NO;
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            >
            $<$<CONFIG:ReleaseEditor>:
            /INCREMENTAL:NO;
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            >
            )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            -g;
            -O0;
            -fno-inline;
            -fstack-protector-strong;
            -D_FORTIFY_SOURCE=2;
            >
            $<$<CONFIG:DebugEditor>:
            -g;
            -O0;
            -fno-inline;
            -fstack-protector-strong;
            -D_FORTIFY_SOURCE=2;
            >
            $<$<CONFIG:Release>:
            -O3;
            -flto;
            -fstack-protector-strong;
            -D_FORTIFY_SOURCE=2;
            >
            $<$<CONFIG:ReleaseEditor>:
            -O3;
            -flto;
            -fstack-protector-strong;
            -D_FORTIFY_SOURCE=2;
            >
            -std=c++20;
            -fpermissive;
            -Wformat;
            -Wformat-security;
            -Wall;
            -Werror;
            -fmax-errors=5;
            -pthread;
            )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            -Wl,-z,relro,-z,now;
            -Wl,-z,noexecstack;
            >
            $<$<CONFIG:DebugEditor>:
            -Wl,-z,relro,-z,now;
            -Wl,-z,noexecstack;
            >
            $<$<CONFIG:Release>:
            -Wl,-z,relro,-z,now;
            -Wl,-z,noexecstack;
            >
            $<$<CONFIG:ReleaseEditor>:
            -Wl,-z,relro,-z,now;
            -Wl,-z,noexecstack;
            >
            )

endif ()

set(libs
        "vulkan-1;"
        "shaderc_shared;"

        "$<$<CONFIG:Debug>:"
        "spirv-cross-cored;"
        "spirv-cross-glsld;"
        "spirv-cross-reflectd;"
        ">"

        "$<$<CONFIG:DebugEditor>:"
        "Editor;"
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
        "Editor;"
        "assimp-vc143-mt;"
        "spirv-cross-core;"
        "spirv-cross-glsl;"
        "spirv-cross-reflect;"
        ">"
        )

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
        Common;
        Editor;
        Renderer;
        GraphicsVulkan;
        SDL3::SDL3;
        SDL3_image::SDL3_image;
        SDL3_mixer::SDL3_mixer;
        SDL3_ttf::SDL3_ttf;
        "${libs}"
        )

target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
        "../3rdParty/ASSIMP/build/x64/lib/Debug;"
        >
        "$<$<CONFIG:DebugEditor>:"
        "../3rdParty/ASSIMP/build/x64/lib/Debug;"
        >
        "$<$<CONFIG:Release>:"
        "../3rdParty/ASSIMP/build/x64/lib/Release"
        >
        "$<$<CONFIG:ReleaseEditor>:"
        "../3rdParty/ASSIMP/build/x64/lib/Release"
        >
        "../3rdParty/Vulkan/lib;"
        )
