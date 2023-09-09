set(PROJECT_NAME BalbinoControl)

add_library(${PROJECT_NAME} STATIC
        ISystem.h
        IResourceManager.h
        ICameraManager.h
        System/System.h
        System/System.cpp
        Resource/ResourceManager.h
        Resource/ResourceManager.cpp
        Input/InputHandler.h
        Input/InputHandler.cpp
        Input/InputButtons.h
        Camera/CameraManager.h
        Camera/CameraManager.cpp
)
 add_dependencies(${PROJECT_NAME}
         DeerUtils
         BambiMath
         FawnVision
         DeerVulkan
         BalbinoScene
         FawnTrace
         BalbinoComponent
         AntlerSerialize

         glm::glm
         imgui
         SDL3::SDL3
 )
set(ROOT_NAMESPACE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Source/Engine/Components
        ${CMAKE_SOURCE_DIR}/Source/Engine/Debugging
        ${CMAKE_SOURCE_DIR}/Source/Engine/Math
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer/API/Vulkan
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer
        ${CMAKE_SOURCE_DIR}/Source/Engine/Scene
        ${CMAKE_SOURCE_DIR}/Source/Serializer
        ${CMAKE_SOURCE_DIR}/Source/Utilities

        PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdParty/GLM
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui
        ${CMAKE_SOURCE_DIR}/3rdParty/SDL/include
        ${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/include
)

include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_precompiled_header(${PROJECT_NAME})
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)
target_compile_definitions(${PROJECT_NAME} PRIVATE "_LIB")

target_link_libraries(${PROJECT_NAME} PRIVATE
        BambiMath
        FawnVision
        DeerVulkan
        BalbinoScene

        glm::glm
        imgui
        SDL3::SDL3
)
