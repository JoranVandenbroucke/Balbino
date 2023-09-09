set(PROJECT_NAME FawnVision)

file(GLOB files *.cpp *.h)

add_library(${PROJECT_NAME} STATIC
        ${files}

        ${CMAKE_SOURCE_DIR}/3rdParty/imgui/backends/imgui_impl_sdl3.cpp
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui/backends/imgui_impl_sdl3.h
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui/backends/imgui_impl_vulkan.cpp
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui/backends/imgui_impl_vulkan.h
)

add_dependencies(${PROJECT_NAME}
        DeerUtils
        BambiMath
        BalbinoComponent
        DeerVulkan
        BalbinoScene

#        EnTT::EnTT
        glm::glm
        imgui
        SDL3::SDL3
)
set(ROOT_NAMESPACE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Source/Engine/Components
        ${CMAKE_SOURCE_DIR}/Source/Engine/Math
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer/API/Vulkan
        ${CMAKE_SOURCE_DIR}/Source/Engine/Scene
        ${CMAKE_SOURCE_DIR}/Source/Utilities

        PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdParty/EnTT/src
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
        DeerVulkan
        BambiMath
        BalbinoScene

        glm::glm
        imgui
        SDL3::SDL3
)