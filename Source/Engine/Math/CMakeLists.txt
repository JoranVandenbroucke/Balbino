set(PROJECT_NAME BambiMath)

add_library(${PROJECT_NAME} STATIC
        BambiMath.h
        BambiMathCore.h
        Random.h
        Random.cpp
        Types.h
)

add_dependencies(${PROJECT_NAME}
        glm::glm
        imgui
)

set(ROOT_NAMESPACE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/Source/Utilities

        ${CMAKE_SOURCE_DIR}/3rdParty/GLM
        ${CMAKE_SOURCE_DIR}/3rdParty/imgui
)

include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_precompiled_header(${PROJECT_NAME})
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)
target_compile_definitions(${PROJECT_NAME} PRIVATE "_LIB")

target_link_libraries(${PROJECT_NAME} PRIVATE
        imgui
)