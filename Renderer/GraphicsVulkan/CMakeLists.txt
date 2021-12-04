set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUGEDITOR   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_DEBUG         "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
    OUTPUT_DIRECTORY_RELEASE       "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/${PROJECT_NAME}/"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

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
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()



























set(PROJECT_NAME GraphicsVulkan)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Base.h"
    "Device.h"
    "FrameBuffer.h"
    "ImageResource.h"
    "ImageView.h"
    "Instance.h"
    "pch.h"
    "Queue.h"
    "Swapchain.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Base.cpp"
    "Device.cpp"
    "FrameBuffer.cpp"
    "ImageResource.cpp"
    "ImageView.cpp"
    "Instance.cpp"
    "pch.cpp"
    "Queue.cpp"
    "Swapchain.cpp"
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

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GraphicsVulkan)

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
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE       "TRUE"
)
################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/Vulkan/Include"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:DebugEditor>:
            /permissive-;
            /MP;
            /sdl;
            /W4;
            /WX
        >
        $<$<CONFIG:Release>:
            /permissive-;
            /MP;
            /Oi;
            /sdl;
            /Gy;
            /W4;
            /WX
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

