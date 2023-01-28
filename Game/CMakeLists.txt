set(PROJECT_NAME Game)

# project setting
add_executable(${PROJECT_NAME} scr/main.cpp resource.rc)
target_precompile_headers(${PROJECT_NAME}
        PRIVATE [["Balbino.h"]])

set(ROOT_NAMESPACE Game)

target_include_directories(${PROJECT_NAME} PRIVATE
        ../Engine/
        ../Common/
        ../3rdParty/SDL2/include
        ../3rdParty/SDL2_image/include
        ../3rdParty/SDL2_mixer/include
        ../3rdParty/SDL2_ttf/include
        )

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "BL_PLATFORM_WINDOWS;"
        "BL_BUILD_LIB;"
        "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        ">"
        "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "BALBINO_EDITOR;"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG;"
        ">"
        "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG;"
        "BALBINO_EDITOR;"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE;"
        "_LIB"
        )

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /Zi;
            /Ob0;
            /Od;
            /RTC1;
            /MDd;
            >
            $<$<CONFIG:DebugEditor>:
            /Zi;
            /Ob0;
            /Od;
            /RTC1;
            /MDd;
            >
            $<$<CONFIG:Release>:
            /Ox;
            /Ob2;
            /Oi;
            /Gy
            >
            $<$<CONFIG:ReleaseEditor>:
            /Ox;
            /Ob2;
            /Oi;
            /Gy
            >
            /MP;
            /std:c++20;
            /permissive-;
            /sdl;
            /W4;
            /WX;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
            )
    target_link_options(${PROJECT_NAME} PRIVATE
            /INCREMENTAL;
            $<$<CONFIG:Debug>:
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE;
            >
            $<$<CONFIG:DebugEditor>:
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE;
            >
            $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /SUBSYSTEM:WINDOWS;
            >
            $<$<CONFIG:ReleaseEditor>:
            /OPT:REF;
            /OPT:ICF;
            /SUBSYSTEM:WINDOWS;
            >
            )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall;
            -Werror;
            -std=c++20;
            )
endif ()

add_dependencies(${PROJECT_NAME}
        Balbino
        )

# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
        Balbino
        )

set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "Balbino"
        )
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
        "../3rdParty/SDL2/lib/x64/;"
        "../3rdParty/SDL2_image/lib/x64/;"
        "../3rdParty/SDL2_mixer/lib/x64/;"
        "../3rdParty/SDL2_ttf/lib/x64/;"
        "../3rdParty/SPIRV-Cross/lib/Release;"
        "../3rdParty/SPIRV-Cross/lib/Debug;"
        "../3rdParty/Vulkan/lib/;"
        "../3rdParty/ASSIMP/build/x64/lib/Debug;"
        "../3rdParty/ASSIMP/build/x64/lib/Release"
        )
message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2/lib/x64/SDL2.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/SDL2_image.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL2_image.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/SDL2_mixer.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL2_mixer.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_ttf/lib/x64/SDL2_ttf.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL2_ttf.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/ASSIMP/build/x64/bin/Debug/assimp-vc143-mtd.dll ${CMAKE_CURRENT_BINARY_DIR}/assimp-vc143-mtd.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/ASSIMP/build/x64/bin/Release/assimp-vc143-mt.dll ${CMAKE_CURRENT_BINARY_DIR}/assimp-vc143-mt.dll
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../../Data/ ${CMAKE_CURRENT_BINARY_DIR}/../Data/
        #######################################################################################################################################################################################################
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/optional/libtiff-5.dll ${CMAKE_CURRENT_BINARY_DIR}/libtiff-5.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/optional/libwebp-7.dll ${CMAKE_CURRENT_BINARY_DIR}/libwebp-7.dll

        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/optional/libmodplug-1.dll ${CMAKE_CURRENT_BINARY_DIR}/libFLAC-8.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/optional/libogg-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libmodplug-1.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/optional/libopus-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libmpg123-0.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/optional/libopusfile-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libogg-0.dll

)