set(PROJECT_NAME FawnEditor)

file(GLOB_RECURSE files *.cpp *.hpp)

add_library(${PROJECT_NAME}
        ${files}
        ${CMAKE_SOURCE_DIR}/3rdParty/mikktspace/mikktspace.c
        ${CMAKE_SOURCE_DIR}/3rdParty/mikktspace/mikktspace.h

)
add_dependencies(${PROJECT_NAME}
        DeerUtils
        BambiMath
        FawnVision
        DeerVulkan
        BalbinoScene
        CervidaeComponents
        BalbinoControl
        DoeDebug
        FawnFiles

        assimp::assimp
        #        EnTT::EnTT
        glm::glm
        gli
        hdrloader
        imgui
        ImGuizmo
        imnodes
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)
set(ROOT_NAMESPACE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Source/Engine/Components
        ${CMAKE_SOURCE_DIR}/Source/Engine/Debugging
        ${CMAKE_SOURCE_DIR}/Source/Engine/Math
        ${CMAKE_SOURCE_DIR}/Source/Engine/Managers
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer/API/Vulkan
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer
        ${CMAKE_SOURCE_DIR}/Source/Engine/Scene
        ${CMAKE_SOURCE_DIR}/Source/FileSystem/includes
        ${CMAKE_SOURCE_DIR}/Source/Utilities

        PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdParty/Assimp/include
        ${CMAKE_BINARY_DIR}/3rdParty/Assimp/include
        ${CMAKE_SOURCE_DIR}/3rdParty/EnTT/src
        ${CMAKE_SOURCE_DIR}/3rdParty/GLM
        ${CMAKE_SOURCE_DIR}/3rdParty/GLI
        ${CMAKE_SOURCE_DIR}/3rdParty/hdrloader
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui
        ${CMAKE_SOURCE_DIR}/3rdParty/ImGuizmo
        ${CMAKE_SOURCE_DIR}/3rdParty/imnodes
        ${CMAKE_SOURCE_DIR}/3rdParty/mikktspace
        ${CMAKE_SOURCE_DIR}/3rdParty/mINI/src
        ${CMAKE_SOURCE_DIR}/3rdParty/SDL/include
        ${CMAKE_SOURCE_DIR}/3rdParty/SDL_mixer/include
        ${CMAKE_SOURCE_DIR}/3rdParty/SDL_image/include
        ${CMAKE_SOURCE_DIR}/3rdParty/SDL_ttf/include
        ${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/include
)

include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_precompiled_header(${PROJECT_NAME})
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)


target_link_libraries(${PROJECT_NAME} PRIVATE
        BambiMath
        FawnVision
        DeerVulkan
        BalbinoScene
        BalbinoControl
        FawnFiles

        glslang$<$<CONFIG:Debug>:d>
        glslang-default-resource-limits$<$<CONFIG:Debug>:d>
        shaderc_util$<$<CONFIG:Debug>:d>
        shaderc_combined$<$<CONFIG:Debug>:d>
        shaderc$<$<CONFIG:Debug>:d>
        spirv-cross-glsl$<$<CONFIG:Debug>:d>
        SPIRV-Tools$<$<CONFIG:Debug>:d>
        SPIRV-Tools-opt$<$<CONFIG:Debug>:d>
        SPIRV$<$<CONFIG:Debug>:d>

        assimp::assimp
        glm::glm
        gli
        hdrloader
        imgui
        ImGuizmo
        imnodes
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)
