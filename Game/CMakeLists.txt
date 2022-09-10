cmake_minimum_required(VERSION 3.22.2)

set(CMAKE_CONFIGURATION_TYPES "Debug;DebugEditor;Release;ReleaseEditor" CACHE STRING "" FORCE)
set(CMAKE_CONFIGURATION_TYPES Debug;DebugEditor;Release;ReleaseEditor;)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set Project Name
set(PROJECT_NAME Game)
project(${PROJECT_NAME})

# sundirectories
add_subdirectory(../Balbino Balbino)

# project setting
add_executable(${PROJECT_NAME} scr/main.cpp resource.rc)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CONFIGURATION_TYPES "Debug;DebugEditor;Release;ReleaseEditor")

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /Od;
            /Zi;
            /Ob0;
            /RTC1;
            /MDd;
            >
            $<$<CONFIG:DebugEditor>:
            /Od;
            /Zi;
            /Ob0;
            /RTC1;
            /MDd;
            >
            $<$<CONFIG:Release>:
            /Ox;
            /Oi-;
            >
            $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Oi-;
            >
            /permissive-;
            /MP;
            /W4;
            /WX;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
            )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /DEBUG:FULL
            /SUBSYSTEM:CONSOLE
            /INCREMENTAL
            >
            $<$<CONFIG:DebugEditor>:
            /DEBUG:FULL
            /SUBSYSTEM:CONSOLE
            /INCREMENTAL
            >
            $<$<CONFIG:Release>:
            /DEBUG
            /SUBSYSTEM:WINDOWS
            >
            $<$<CONFIG:ReleaseEditor>:
            /DEBUG
            /SUBSYSTEM:WINDOWS
            >
            )
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

target_precompile_headers(${PROJECT_NAME}
        PRIVATE [["Balbino.h"]])

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "BL_PLATFORM_WINDOWS;"
        "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        ">"
        "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG;"
        ">"
        "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG;"
        ">"
        "UNICODE;"
        "_UNICODE"
        )

# includes
target_include_directories(${PROJECT_NAME}
        PRIVATE ../Balbino/
        PRIVATE ../Common/
        PRIVATE ../3rdParty/SDL2/include
        PRIVATE ../3rdParty/SDL2_image/include
        PRIVATE ../3rdParty/SDL2_mixer/include
        PRIVATE ../3rdParty/SDL2_ttf/include
        PRIVATE ../3rdParty/VLD/include
        )

# libaries
target_link_directories(${PROJECT_NAME} PRIVATE
        ../3rdParty/VLD/lib/Win64
        )
target_link_libraries(${PROJECT_NAME} PRIVATE
        "Balbino"
        "vld"
        )

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Balbino/Balbino.dll ${CMAKE_CURRENT_BINARY_DIR}/Balbino.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2/lib/x64/SDL2.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/SDL2_image.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL2_image.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/libjpeg-9.dll ${CMAKE_CURRENT_BINARY_DIR}/libjpeg-9.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/libpng16-16.dll ${CMAKE_CURRENT_BINARY_DIR}/libpng16-16.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/libtiff-5.dll ${CMAKE_CURRENT_BINARY_DIR}/libtiff-5.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/libwebp-7.dll ${CMAKE_CURRENT_BINARY_DIR}/libwebp.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/zlib1.dll ${CMAKE_CURRENT_BINARY_DIR}/zlib1.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/SDL2_mixer.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL2_mixer.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/libFLAC-8.dll ${CMAKE_CURRENT_BINARY_DIR}/libFLAC-8.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/libmodplug-1.dll ${CMAKE_CURRENT_BINARY_DIR}/libmodplug-1.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/libmpg123-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libmpg123-0.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/libogg-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libogg-0.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/libopus-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libopus-0.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/libopusfile-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libopusfile-0.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/libvorbis-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libvorbis-0.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/libvorbisfile-3.dll ${CMAKE_CURRENT_BINARY_DIR}/libvorbisfile-3.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_ttf/lib/x64/SDL2_ttf.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL2_ttf.dll
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/VLD/bin/Win64/ ${CMAKE_CURRENT_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/VLD/bin/Win64/ ${CMAKE_CURRENT_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../../Data/ ${CMAKE_CURRENT_BINARY_DIR}/../Data/

        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/ASSIMP/build/x64/bin/Debug/assimp-vc142-mtd.dll ${CMAKE_CURRENT_BINARY_DIR}/assimp-vc142-mtd.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/ASSIMP/build/x64/bin/Release/assimp-vc142-mt.dll ${CMAKE_CURRENT_BINARY_DIR}/assimp-vc142-mt.dll
)
