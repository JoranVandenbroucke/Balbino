cmake_minimum_required(VERSION 3.21.1)
set(CMAKE_CONFIGURATION_TYPES Debug;DebugEditor;Release;ReleaseEditor;)

# Set Project Name
project(ImGui)

# get source files
file(GLOB sourceImGui
        *.cpp *.h
        **/*.cpp **/*.h
        )

# project setting
add_library(ImGui ${sourceImGui})
set_property(TARGET ImGui PROPERTY CXX_STANDARD 20)
set_property(TARGET ImGui PROPERTY C_STANDARD 17)
set_property(TARGET ImGui PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ImGui PROPERTY C_STANDARD_REQUIRED ON)

if (MSVC)
    target_compile_options(ImGui PRIVATE
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

    target_compile_definitions(ImGui PRIVATE
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
            )
    target_link_options(ImGui PRIVATE
            $<$<CONFIG:Debug>:
            /DEBUG:FULL
            /SUBSYSTEM:CONSOLE
            /INCREMENTAL;
            >
            $<$<CONFIG:DebugEditor>:
            /DEBUG:FULL
            /SUBSYSTEM:CONSOLE
            /INCREMENTAL
            >
            $<$<CONFIG:Release>:
            /DEBUG
            /SUBSYSTEM:WINDOWS
            >
            $<$<CONFIG:ReleaseEditor>:
            /DEBUG
            /SUBSYSTEM:WINDOWS
            >

            )
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

target_include_directories(ImGui
        PUBLIC ../../3rdParty/Vulkan/Include
        PUBLIC ../../3rdParty/SDL2/include
        )
target_link_directories(ImGui
        PRIVATE "../../3rdParty/SDL2/lib/x64/"
        PRIVATE "../../3rdParty/Vulkan/lib/"
        )
target_link_libraries(ImGui
        PRIVATE
        SDL2
        vulkan-1
        )