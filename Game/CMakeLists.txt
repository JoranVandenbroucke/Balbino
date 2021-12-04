set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)



























set(PROJECT_NAME Game)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "scr/pch.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/main.cpp"
    "scr/pch.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Game)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Balbino;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "_CRTDBG_MAP_ALLOC;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG"
    ">"
    "BL_PLATFORM_WINDOWS;"
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
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:DebugEditor>:
            /SUBSYSTEM:CONSOLE;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:ReleaseEditor>:
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG:FULL
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command_if(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMANDS
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND         $<CONFIG:Debug> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND   $<CONFIG:DebugEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND       $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${OUTPUT_DIRECTORY}>..\\Balbino\\Balbino.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2\\lib\\x64\\SDL2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_image\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_ttf\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\SDL2_mixer\\lib\\x64\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
    COMMAND $<CONFIG:ReleaseEditor> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>3rdParty\\Assimp\\bin\\$<CONFIG>\\*.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" /D /Y
)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Balbino
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Balbino
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "SDL2;"
    "SDL2main;"
    "SDL2_image;"
    "SDL2_ttf;"
    "SDL2_mixer;"
    "vulkan-1"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/TinyDDSLoader"
)

