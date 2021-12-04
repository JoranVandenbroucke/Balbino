set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
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
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)



























set(PROJECT_NAME Editor)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/Interface.h"
    "scr/pch.h"
    "scr/Windows/GameView.h"
    "scr/Windows/MainScreen.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Interface.cpp"
    "scr/pch.cpp"
    "scr/Windows/GameView.cpp"
    "scr/Windows/MainScreen.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Editor)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASEEDITOR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
    INTERPROCEDURAL_OPTIMIZATION_RELEASEEDITOR "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include"
)

################################################################################
# Compile definitions
################################################################################
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
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Ox;
            /Oi;
            /Gy
        >
        $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi;
            /Gy
        >
        /permissive-;
        /MP;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND         $<CONFIG:Debug> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> ) ELSE (
    COMMAND         $<CONFIG:Debug> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND         $<CONFIG:Debug> )
    COMMAND   $<CONFIG:DebugEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> ) ELSE (
    COMMAND   $<CONFIG:DebugEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND   $<CONFIG:DebugEditor> )
    COMMAND       $<CONFIG:Release> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> ) ELSE (
    COMMAND       $<CONFIG:Release> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND       $<CONFIG:Release> )
    COMMAND $<CONFIG:ReleaseEditor> IF EXIST "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" (
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> ) ELSE (
    COMMAND $<CONFIG:ReleaseEditor> mkdir "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game"
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Game" /E /H /C /I /Y
    COMMAND $<CONFIG:ReleaseEditor> )
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vld"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Visual Leak Detector/lib/x64"
)

