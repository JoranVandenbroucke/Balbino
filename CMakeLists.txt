cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)

project(Balbino LANGUAGES C CXX VERSION "0.1.0")


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(3rdParty/GLM)
add_subdirectory(3rdParty/imgui)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library." FORCE)
set(SDL3MIXER_VENDORED ON CACHE BOOL "Use vendored third-party libraries" FORCE)
set(GLI_TEST_ENABLE OFF CACHE BOOL "Build unit tests" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build the SDL3_test library" FORCE)

add_subdirectory(3rdParty/SDL)
add_subdirectory(3rdParty/SDL_mixer)

if (BALBINO_EDITOR)
    set(ASSIMP_BUILD_ZLIB ON)
    set(SDL3IMAGE_VENDORED ON CACHE BOOL "Use vendored third-party libraries" FORCE)
    set(SDL3TTF_VENDORED ON CACHE BOOL "Use vendored third-party libraries" FORCE)
    add_subdirectory(3rdParty/Assimp)
    add_subdirectory(3rdParty/GLI)
    add_subdirectory(3rdParty/hdrloader)
    add_subdirectory(3rdParty/ImGuizmo)
    add_subdirectory(3rdParty/imnodes)
    add_subdirectory(3rdParty/SDL_image)
    add_subdirectory(3rdParty/SDL_ttf)

    add_subdirectory(Source/Editor)
endif ()
#set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Install documentation alongside library" FORCE)
#add_subdirectory(3rdParty/Catch2)

add_subdirectory(Source/Utilities)
add_subdirectory(Source/Memory)
add_subdirectory(Source/Engine/Math)
add_subdirectory(Source/Engine/Debugging)
add_subdirectory(Source/Engine/Renderer/API/Vulkan)
add_subdirectory(Source/Engine/Components)
add_subdirectory(Source/Engine/Scene)
add_subdirectory(Source/Engine/Renderer)
add_subdirectory(Source/Serializer)
add_subdirectory(Source/Engine/Managers)
add_subdirectory(Source/Application)
add_subdirectory(Source/Game)
add_subdirectory(Tests)
add_subdirectory(blink)
