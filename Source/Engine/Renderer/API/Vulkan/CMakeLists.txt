set(PROJECT_NAME DeerVulkan)

add_library(${PROJECT_NAME} STATIC
        Base.h
        Buffer.cpp
        Buffer.h
        CommandPool.cpp
        CommandPool.h
        DeerVulkan.h
        DeerVulkanCore.h
        DescriptorSet.cpp
        DescriptorSet.h
        Device.cpp
        Device.h
        Fence.cpp
        Fence.h
        FrameBuffer.cpp
        FrameBuffer.h
        ImageResource.cpp
        ImageResource.h
        ImageView.cpp
        ImageView.h
        Instance.cpp
        Instance.h
        Queue.cpp
        Queue.h
        RenderPass.cpp
        RenderPass.h
        Sampler.cpp
        Sampler.h
        Semaphore.cpp
        Semaphore.h
        Shader.cpp
        Shader.h
        ShaderPipeline.cpp
        ShaderPipeline.h
        Swapchain.cpp
        Swapchain.h
)

add_dependencies(${PROJECT_NAME}
        DeerUtils
        BambiMath
)

set(ROOT_NAMESPACE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Source/Engine/Math
        ${CMAKE_SOURCE_DIR}/Source/Utilities

        PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdParty/GLM
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui
        ${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/include
)

include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_precompiled_header(${PROJECT_NAME})
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)
target_compile_definitions(${PROJECT_NAME} PRIVATE "_LIB")

target_link_libraries(${PROJECT_NAME} PRIVATE
        BambiMath

        vulkan-1

        spirv-cross-core$<$<CONFIG:Debug>:d>
        spirv-cross-glsl$<$<CONFIG:Debug>:d>
        spirv-cross-reflect$<$<CONFIG:Debug>:d>
)

target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib
)