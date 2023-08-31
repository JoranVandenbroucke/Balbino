set(PROJECT_NAME BalbinoScene)

add_library(${PROJECT_NAME} STATIC
        Camera.h
        Camera.cpp
        Entity.h
        Entity.cpp
        Scene.h
        Scene.cpp
)
add_dependencies(${PROJECT_NAME}
        DeerUtils
        BambiMath
        BalbinoComponent

#        EnTT::EnTT
        glm::glm
        imgui
)
set(ROOT_NAMESPACE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Source/Engine/Components
        ${CMAKE_SOURCE_DIR}/Source/Engine/Math
        ${CMAKE_SOURCE_DIR}/Source/Utilities

        PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdParty/EnTT/src
        ${CMAKE_SOURCE_DIR}/3rdParty/GLM
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui
)
include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_precompiled_header(${PROJECT_NAME})
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)
target_compile_definitions(${PROJECT_NAME} PRIVATE "_LIB")

target_link_libraries(${PROJECT_NAME} PRIVATE
        BambiMath

        glm::glm
        imgui
)