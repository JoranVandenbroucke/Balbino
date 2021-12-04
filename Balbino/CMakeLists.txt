set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /Od
        >
        $<$<CONFIG:Debug>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od
        >
        $<$<CONFIG:DebugEditor>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:DebugEditor>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)



























set(PROJECT_NAME Balbino)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Balbino.h"
    "scr/Application.h"
    "scr/Camera.h"
    "scr/Core.h"
    "scr/EntryPoint.h"
    "scr/Input/InputButtons.h"
    "scr/Input/InputHandler.h"
    "scr/Managers/CameraManager.h"
    "scr/Managers/Manager.h"
    "scr/Managers/MaterialManager.h"
    "scr/Managers/MeshManager.h"
    "scr/Managers/ShaderManager.h"
    "scr/Managers/Singleton.h"
    "scr/Managers/TextureManager.h"
    "scr/pch.h"
    "scr/Renderer/IndexBuffer.h"
    "scr/Renderer/Material.h"
    "scr/Renderer/Mesh.h"
    "scr/Renderer/MeshMetadata.h"
    "scr/Renderer/Renderer.h"
    "scr/Renderer/Shader.h"
    "scr/Renderer/Texture.h"
    "scr/Renderer/TextureSampler.h"
    "scr/Renderer/UniformBufferObject.h"
    "scr/Renderer/Vertex.h"
    "scr/Renderer/VertexBuffer.h"
    "scr/Scene.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "scr/Application.cpp"
    "scr/Camera.cpp"
    "scr/Input/InputButtons.cpp"
    "scr/Input/InputHandler.cpp"
    "scr/Managers/CameraManager.cpp"
    "scr/Managers/Manager.cpp"
    "scr/Managers/MaterialManager.cpp"
    "scr/Managers/MeshManager.cpp"
    "scr/Managers/ShaderManager.cpp"
    "scr/Managers/Singleton.cpp"
    "scr/Managers/TextureManager.cpp"
    "scr/pch.cpp"
    "scr/Renderer/IndexBuffer.cpp"
    "scr/Renderer/Material.cpp"
    "scr/Renderer/Mesh.cpp"
    "scr/Renderer/Renderer.cpp"
    "scr/Renderer/Shader.cpp"
    "scr/Renderer/Texture.cpp"
    "scr/Renderer/TextureSampler.cpp"
    "scr/Renderer/VertexBuffer.cpp"
    "scr/Scene.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "scr/pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Balbino)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_image/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_mixer/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_net/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2_ttf/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/SDL2/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/glm/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Assimp/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Vulkan/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/gli;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Editor;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Imgui"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "BL_EDITOR;"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "BL_EDITOR;"
        "NDEBUG"
    ">"
    "BL_BUILD_DLL;"
    "BL_PLATFORM_WINDOWS;"
    "_CONSOLE;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od
        >
        $<$<CONFIG:DebugEditor>:
            /Od
        >
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
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:DebugEditor>:
            /DEBUG:FULL;
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        $<$<CONFIG:ReleaseEditor>:
            /DEBUG;
            /OPT:REF;
            /LTCG:incremental;
            /OPT:ICF;
            /INCREMENTAL:NO
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

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    Editor
    Imgui
)

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    Editor
    Imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "sdl2;"
    "sdl2_image;"
    "sdl2_ttf;"
    "sdl2_mixer;"
    "vulkan-1;"
    "assimp-vc142-mt"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib/$<CONFIG>"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "$<$<CONFIG:ReleaseEditor>:"
        "${CMAKE_SOURCE_DIR}/3rdParty/DDS_loader/lib"
    ">"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_image/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_ttf/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/SDL2_mixer/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib/x64;"
    "${CMAKE_SOURCE_DIR}/3rdParty/Assimp/lib/$<CONFIG>"
)

