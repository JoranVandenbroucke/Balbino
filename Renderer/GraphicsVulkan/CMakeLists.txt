cmake_minimum_required(VERSION 3.22.2)
set(CMAKE_CONFIGURATION_TYPES Debug;DebugEditor;Release;ReleaseEditor;)

# Set Project Name
project(BalVulkan)

file(GLOB sourceGV *.cpp *.h)

add_library(BalVulkan STATIC ${sourceGV})
set_property(TARGET BalVulkan PROPERTY CXX_STANDARD 23)
set_property(TARGET BalVulkan PROPERTY C_STANDARD 17)
set_property(TARGET BalVulkan PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET BalVulkan PROPERTY C_STANDARD_REQUIRED ON)

if (MSVC)
    target_compile_options(BalVulkan PRIVATE
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
            /std:c++latest;
            /MP;
            /W4;
            /WX
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
            )
    target_link_options(BalVulkan PRIVATE
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

target_compile_definitions(${PROJECT_NAME} PRIVATE
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
        "_LIBS;"
        "_MBCS;"
        )

target_precompile_headers(BalVulkan
        PRIVATE [["algorithm"]]
        PRIVATE [["array"]]
        PRIVATE [["cmath"]]
        PRIVATE [["cstdint"]]
        PRIVATE [["format"]]
        PRIVATE [["iostream"]]
        PRIVATE [["unordered_map"]]
        PRIVATE [["vector"]]
        PRIVATE [["glm/glm.hpp"]])

target_include_directories(BalVulkan
        PRIVATE "../../3rdParty/GLM"
        PRIVATE "../../3rdParty/SPIRV-Cross/include"
        PRIVATE "../../3rdParty/Vulkan/include"
        )