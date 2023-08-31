set(PROJECT_NAME AntlerTest)

add_executable(${PROJECT_NAME} test_main.cpp AntlerTestCore.h GraphicDeviceTest.cpp)

set(ROOT_NAMESPACE ${PROJECT_NAME})

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdParty/Catch2 Catch2)

add_dependencies(${PROJECT_NAME}
        Catch2
        imgui
        FawnVision
        DeerVulkan
        BambiMath
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/3rdParty/Catch2/src
        ${CMAKE_SOURCE_DIR}/Source/Engine/Components
        ${CMAKE_SOURCE_DIR}/Source/Engine/Debugging
        ${CMAKE_SOURCE_DIR}/Source/Engine/Managers
        ${CMAKE_SOURCE_DIR}/Source/Engine/Math
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer/API/Vulkan
        ${CMAKE_SOURCE_DIR}/Source/Engine/Scene
        ${CMAKE_SOURCE_DIR}/Source/Serializer
        ${CMAKE_SOURCE_DIR}/Source/Utilities
)

include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_precompiled_header(${PROJECT_NAME})
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)
target_compile_definitions(${PROJECT_NAME} PRIVATE "_LIB")

target_link_libraries(${PROJECT_NAME} PRIVATE
        Catch2::Catch2
        imgui
        glm::glm
        FawnVision
        DeerVulkan
        BambiMath
)