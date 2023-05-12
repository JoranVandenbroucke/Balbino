set(PROJECT_NAME Deer)

# project setting
add_executable(${PROJECT_NAME} scr/main.cpp resource.rc)
target_precompile_headers(${PROJECT_NAME} PRIVATE
        [["Balbino.h"]]
        )
add_dependencies(${PROJECT_NAME}
        Balbino
        )

set(ROOT_NAMESPACE Game)

target_include_directories(${PROJECT_NAME} PRIVATE
        ../Engine
        ../Engine/Common
        ../3rdParty/SDL/include
        )

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "BL_PLATFORM_WINDOWS;"
        "BL_BUILD_LIB;"
        "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "_CONSOLE;"
        ">"
        "$<$<CONFIG:DebugEditor>:"
        "_DEBUG;"
        "_CONSOLE;"
        "BALBINO_EDITOR;"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG;"
        ">"
        "$<$<CONFIG:ReleaseEditor>:"
        "NDEBUG;"
        "BALBINO_EDITOR;"
        ">"
        "UNICODE;"
        "_UNICODE;"
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
            /std:c++20;
            /permissive-;
            /sdl;
            /W4;
            /WX;
            /MP;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
            )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /INCREMENTAL;
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE;
            >
            $<$<CONFIG:DebugEditor>:
            /INCREMENTAL;
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE;
            >
            $<$<CONFIG:Release>:
            /INCREMENTAL:NO;
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            >
            $<$<CONFIG:ReleaseEditor>:
            /INCREMENTAL:NO;
            /DEBUG:FULL;
            /SUBSYSTEM:WINDOWS;
            /OPT:REF;
            /OPT:ICF;
            >
            )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            -g;
            -O0;
            -fno-inline;
            -fstack-protector-strong;
            -D_FORTIFY_SOURCE=2;
            >
            $<$<CONFIG:DebugEditor>:
            -g;
            -O0;
            -fno-inline;
            -fstack-protector-strong;
            -D_FORTIFY_SOURCE=2;
            >
            $<$<CONFIG:Release>:
            -O3;
            -flto;
            -fstack-protector-strong;
            -D_FORTIFY_SOURCE=2;
            >
            $<$<CONFIG:ReleaseEditor>:
            -O3;
            -flto;
            -fstack-protector-strong;
            -D_FORTIFY_SOURCE=2;
            >
            -std=c++20;
            -fpermissive;
            -Wformat;
            -Wformat-security;
            -Wall;
            -Werror;
            -fmax-errors=5;
            -pthread;
            )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            -Wl,-z,relro,-z,now;
            -Wl,-z,noexecstack;
            >
            $<$<CONFIG:DebugEditor>:
            -Wl,-z,relro,-z,now;
            -Wl,-z,noexecstack;
            >
            $<$<CONFIG:Release>:
            -Wl,-z,relro,-z,now;
            -Wl,-z,noexecstack;
            >
            $<$<CONFIG:ReleaseEditor>:
            -Wl,-z,relro,-z,now;
            -Wl,-z,noexecstack;
            >
            )
endif ()

add_dependencies(${PROJECT_NAME}
        Balbino
        )
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PRIVATE
        Balbino
        )
target_link_directories(${PROJECT_NAME} PRIVATE
        "../3rdParty/SDL/lib/x64/;"
        "../3rdParty/SDL_image/lib/x64/;"
        "../3rdParty/SDL_mixer/lib/x64/;"
        "../3rdParty/SDL_ttf/lib/x64/;"
        "../3rdParty/Vulkan/lib/;"
        "../3rdParty/ASSIMP/build/x64/lib/Debug;"
        "../3rdParty/ASSIMP/build/x64/lib/Release"
        )

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../Engine/SDL/SDL3.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL3.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../Engine/SDL_image/SDL3_image.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL3_image.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../Engine/SDL_mixer/SDL3_mixer.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL3_mixer.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../Engine/SDL_ttf/SDL3_ttf.dll ${CMAKE_CURRENT_BINARY_DIR}/SDL3_ttf.dll
#        $<$<CONFIG:DebugEditor>:
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/ASSIMP/build/x64/bin/Debug/assimp-vc143-mtd.dll ${CMAKE_CURRENT_BINARY_DIR}/assimp-vc143-mtd.dll
#        >
#        $<$<CONFIG:ReleaseEditor>:
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/ASSIMP/build/x64/bin/Release/assimp-vc143-mt.dll ${CMAKE_CURRENT_BINARY_DIR}/assimp-vc143-mt.dll
#        >
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/../../Data/ ${CMAKE_CURRENT_BINARY_DIR}/../Data/
        #######################################################################################################################################################################################################
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/optional/libtiff-5.dll ${CMAKE_CURRENT_BINARY_DIR}/libtiff-5.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_image/lib/x64/optional/libwebp-7.dll ${CMAKE_CURRENT_BINARY_DIR}/libwebp-7.dll
        #
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/optional/libmodplug-1.dll ${CMAKE_CURRENT_BINARY_DIR}/libFLAC-8.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/optional/libogg-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libmodplug-1.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/optional/libopus-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libmpg123-0.dll
        #        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../../3rdParty/SDL2_mixer/lib/x64/optional/libopusfile-0.dll ${CMAKE_CURRENT_BINARY_DIR}/libogg-0.dll

)