set(PROJECT_NAME GraphicsVulkan)

file(GLOB sourceV
        *.cpp *.h
        )

add_library(${PROJECT_NAME} STATIC ${sourceV})
target_precompile_headers(${PROJECT_NAME} PRIVATE
        [["algorithm"]]
        [["array"]]
        [["cmath"]]
        [["cstdint"]]
        [["format"]]
        [["iostream"]]
        [["unordered_map"]]
        [["vector"]]
        [["glm/glm.hpp"]])

set(ROOT_NAMESPACE GraphicsVulkan)

target_include_directories(${PROJECT_NAME} PRIVATE
        "../../3rdParty/GLM"
        "../../3rdParty/SPIRV-Cross/include"
        "../../3rdParty/Vulkan/include"
        )

target_compile_definitions(${PROJECT_NAME} PRIVATE
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
            -Wall;
            -Werror;
            -std=gnu++2b;
            )
endif ()
