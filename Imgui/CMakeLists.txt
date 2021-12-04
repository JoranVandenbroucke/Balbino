set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:ReleaseEditor>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:Release>:
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
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:ReleaseEditor>:
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
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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



























set(PROJECT_NAME Imgui)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "backends/imgui_impl_sdl.h"
    "backends/imgui_impl_vulkan.h"
    "GraphEditor.h"
    "imconfig.h"
    "ImCurveEdit.h"
    "ImGradient.h"
    "imgui.h"
    "imgui_internal.h"
    "ImGuizmo.h"
    "ImSequencer.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "ImZoomSlider.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "backends/imgui_impl_sdl.cpp"
    "backends/imgui_impl_vulkan.cpp"
    "GraphEditor.cpp"
    "ImCurveEdit.cpp"
    "ImGradient.cpp"
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "ImGuizmo.cpp"
    "ImSequencer.cpp"
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

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include"
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
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        $<$<CONFIG:ReleaseEditor>:
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
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:CONSOLE
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

