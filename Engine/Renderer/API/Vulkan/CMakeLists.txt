set(PROJECT_NAME GraphicsVulkan)

file(GLOB source
        *.cpp *.h
        )

add_library(${PROJECT_NAME} STATIC ${source})
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

set(ROOT_NAMESPACE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        ../../../../3rdParty/GLM
        ../../../../3rdParty/Vulkan/include
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
        "NDEBUG"
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
