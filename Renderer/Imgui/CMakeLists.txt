set(PROJECT_NAME Imgui)

# get source files
file(GLOB sourcemGui
        *.cpp *.h
        **/*.cpp **/*.h
        )

# project setting
add_library(${PROJECT_NAME} ${sourcemGui})


set(ROOT_NAMESPACE Imgui)

target_include_directories(${PROJECT_NAME} PUBLIC
        ../../3rdParty/Vulkan/Include
        ../../3rdParty/SDL2/include
        ../../3rdParty/GLM/
        )

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
        "_DEBUG"
        ">"
        "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
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
            /MP;
            /std:c++20;
            /permissive-;
            /sdl;
            /W4;
            /WX;
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
            $<$<CONFIG:Debug>:
            -O0
            >
            $<$<CONFIG:DebugEditor>:
            -O0
            >
            $<$<CONFIG:Release>:
            -Ofast -ffunction-sections -fdata-sections
            >
            $<$<CONFIG:ReleaseEditor>:
            -Ofast -ffunction-sections -fdata-sections
            >
            -Wformat-security
            -Wpedantic
            -Werror
            -pedantic-errors
            -std=c++20
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
            )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
            --gc-sections
            -fipa-icf
            >
            $<$<CONFIG:ReleaseEditor>:
            --gc-sections
            -fipa-icf
            >
            --subsystem,console
            )
endif ()
