set(PROJECT_NAME Editor)

# get source files
file(GLOB_RECURSE source
        **/*.cpp **/*.h **/*.c
        )

# project setting
add_library(${PROJECT_NAME} STATIC ${source})
add_dependencies(${PROJECT_NAME}
        Common
        GraphicsVulkan
        )

set(ROOT_NAMESPACE Editor)

target_include_directories(${PROJECT_NAME} PRIVATE
        ../Common
        ../Renderer/
        ../Renderer/API/Vulkan
        ../../3rdParty/ASSIMP/include
        ../../3rdParty/EnTT/single_include
        ../../3rdParty/GLM
        ../../3rdParty/GLI
        ../../3rdParty/mINI/src
        ../../3rdParty/SDL/include
        ../../3rdParty/SDL_image/include
        ../../3rdParty/SDL_mixer/include
        ../../3rdParty/SDL_ttf/include
        ../../3rdParty/Vulkan/Include
        )
target_compile_definitions(${PROJECT_NAME} PRIVATE
        "BL_PLATFORM_WINDOWS"
        "BL_BUILD_LIB"
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

        "$<$<CONFIG:DebugEditor>:"
        "BALBINO_EDITOR;"
        ">"
        "$<$<CONFIG:ReleaseEditor>:"
        "BALBINO_EDITOR;"
        ">"
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
            /MP;
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