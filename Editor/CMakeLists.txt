set(PROJECT_NAME Editor)

# get source files
file(GLOB sourceE
        scr/*.cpp scr/*.h
        scr/EditorGUI/*.cpp scr/EditorGUI/*.h
        scr/Tools/FilesSystem/*.cpp scr/Tools/FilesSystem/*.h
        scr/Tools/FilesSystem/Inporter/*.cpp scr/Tools/FilesSystem/Inporter/*.h scr/Tools/FilesSystem/Inporter/*.c
        scr/Windows/*.cpp scr/Windows/*.h
        scr/Windows/Nodes/*.cpp scr/Windows/Nodes/*.h
        scr/Windows/Nodes/Colour/*.cpp scr/Windows/Nodes/Colour/*.h
        scr/Windows/Nodes/Math/*.cpp scr/Windows/Nodes/Math/*.h
        scr/Windows/Nodes/Shaders/*.cpp scr/Windows/Nodes/Shaders/*.h
        scr/Windows/Nodes/Textures/*.cpp scr/Windows/Nodes/Textures/*.h
        )

# project setting
add_library(${PROJECT_NAME} ${sourceE})

set(ROOT_NAMESPACE Editor)

target_include_directories(${PROJECT_NAME} PRIVATE
        ../3rdParty/SDL2/include
        ../3rdParty/SDL2_image/include
        ../3rdParty/SDL2_mixer/include
        ../3rdParty/SDL2_ttf/include
        ../3rdParty/EnTT/single_include
        ../3rdParty/ASSIMP/include
        ../3rdParty/ASSIMP/out/build/x64-Debug/include
        ../3rdParty/ASSIMP/out/build/windows-default/include
        ../3rdParty/GLM
        ../3rdParty/mINI/src/
        ../3rdParty/GLI
        ../3rdParty/Vulkan/Include
        ../Renderer/GraphicsVulkan
        ../Renderer/Imgui
        ../Common
        )
target_compile_definitions(${PROJECT_NAME} PRIVATE
        "BL_PLATFORM_WINDOWS"
        "BL_BUILD_LIB"
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

add_dependencies(${PROJECT_NAME}
        Common
        GraphicsVulkan
        Imgui
        )

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
        Common
        GraphicsVulkan
        Imgui
        )