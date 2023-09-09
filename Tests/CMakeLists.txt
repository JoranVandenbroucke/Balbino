set(PROJECT_NAME AntlerTest)

add_executable(${PROJECT_NAME} test_main.cpp AntlerTestCore.h GraphicDeviceTest.cpp)

set(ROOT_NAMESPACE ${PROJECT_NAME})

set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Install documentation alongside library" FORCE)
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
target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/SUBSYSTEM:CONSOLE>
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        Catch2::Catch2
        imgui
        glm::glm
        FawnVision
        DeerVulkan
        BambiMath

        vulkan-1
)

target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Catch2::Catch2>
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE:SDL3_mixer::SDL3_mixer>
        $<$<BOOL:${BALBINO_EDITOR}>:$<TARGET_FILE:SDL3_image::SDL3_image>>
        $<$<BOOL:${BALBINO_EDITOR}>:$<TARGET_FILE:SDL3_ttf::SDL3_ttf>>
        $<$<BOOL:${BALBINO_EDITOR}>:$<TARGET_FILE:assimp::assimp>>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data  # Source directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Data  # Destination directory (../Data)
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME}
        TEST_PREFIX ""
        REPORTER "xml::out=-"
        OUTPUT_DIR "test_results"
)
