set(PROJECT_NAME AlbinoApp)

add_library(${PROJECT_NAME} STATIC
        EntryPoint.h
        Application.h
        Application.cpp
        Balbino.h
)
add_dependencies(${PROJECT_NAME}
        BalbinoControl
        BambiMath
        FawnTrace
        FawnVision
        DeerVulkan
        BalbinoScene
        BalbinoComponent
        DeerUtils

        glm::glm
        #        EnTT::EnTT
        imgui
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer

)
if (BALBINO_EDITOR)
    add_dependencies(${PROJECT_NAME}
            FawnEditor
            assimp::assimp
            ImGuizmo
            imnodes
    )
endif ()

set(ROOT_NAMESPACE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Source/Engine/Components
        ${CMAKE_SOURCE_DIR}/Source/Engine/Debugging
        ${CMAKE_SOURCE_DIR}/Source/Engine/Math
        ${CMAKE_SOURCE_DIR}/Source/Engine/Managers
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer/API/Vulkan
        ${CMAKE_SOURCE_DIR}/Source/Engine/Renderer
        ${CMAKE_SOURCE_DIR}/Source/Engine/Scene
        ${CMAKE_SOURCE_DIR}/Source/Utilities

        PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdParty/EnTT/src
        ${CMAKE_SOURCE_DIR}/3rdParty/GLM
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui
        ${CMAKE_SOURCE_DIR}/3rdParty/mINI/src
        ${CMAKE_SOURCE_DIR}/3rdParty/SDL/include
        ${CMAKE_SOURCE_DIR}/3rdParty/SDL_mixer/include
        ${CMAKE_SOURCE_DIR}/3rdParty/Vulkan/include

        PRIVATE
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/Source/Editor>
        PUBLIC
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/Assimp/include>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_BINARY_DIR}/3rdParty/Assimp/include>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/EnTT/src>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/GLI>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/hdrloader>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/ImGuizmo>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/imnodes>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/mikktspace>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/SDL_image/include>
        $<$<BOOL:${BALBINO_EDITOR}>:${CMAKE_SOURCE_DIR}/3rdParty/SDL_ttf/include>
)

include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_precompiled_header(${PROJECT_NAME})
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)
target_compile_definitions(${PROJECT_NAME} PRIVATE "_LIB")

target_link_libraries(${PROJECT_NAME} PRIVATE
        BalbinoControl
        BambiMath
        FawnVision
        DeerVulkan
        BalbinoScene

        glm::glm
        #        EnTT::EnTT
        imgui
        SDL3::SDL3
        SDL3_mixer::SDL3_mixer

        $<$<BOOL:${BALBINO_EDITOR}>:FawnEditor>
)
