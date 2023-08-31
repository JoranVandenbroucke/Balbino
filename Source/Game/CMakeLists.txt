set(PROJECT_NAME Deer)

add_executable(${PROJECT_NAME} main.cpp)

set(ROOT_NAMESPACE ${PROJECT_NAME})

add_dependencies(${PROJECT_NAME}
        AlbinoApp
        BalbinoControl
        FawnVision
        FawnTrace
        DeerUtils
        DeerVulkan
        BalbinoScene

        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        imgui
)

if (BALBINO_EDITOR)
    add_dependencies(${PROJECT_NAME}
            FawnEditor
            assimp::assimp
            ImGuizmo
            imnodes
    )
endif ()
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Source/Application
        ${CMAKE_SOURCE_DIR}/Source/Engine/Debugging
        ${CMAKE_SOURCE_DIR}/Source/Engine/Managers
        ${CMAKE_SOURCE_DIR}/Source/Utilities

        ${CMAKE_SOURCE_DIR}/3rdParty/SDL/include
)

include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)

target_precompile_headers(${PROJECT_NAME} PRIVATE
        [["Balbino.h"]]
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        AlbinoApp
        BalbinoControl
        FawnVision
        DeerVulkan
        BalbinoScene

        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
)
target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/lib
)


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # Installs the executable in the binary directory
)
install(FILES
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE:SDL3_mixer::SDL3_mixer>
        DESTINATION ${CMAKE_INSTALL_BINDIR}  # Install the DLLs in the binary directory
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
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