function(setup_sdl3 target_name)
    if(NOT TARGET SDL3::SDL3)
        # Strategy 1: Try system find_package
        find_package(SDL3 QUIET)

        if(SDL3_FOUND)
            message(STATUS "SDL3 found via find_package")
        elseif(EXISTS "${CMAKE_SOURCE_DIR}/3rdParty/SDL3/CMakeLists.txt")
            # Strategy 2: Look in 3rdParty/SDL3
            message(STATUS "SDL3 found in ${CMAKE_SOURCE_DIR}/3rdParty/SDL3")

            # Disable all optional features for minimal build
            set(SDL_SHARED OFF CACHE BOOL "Build SDL3 as shared library" FORCE)
            set(SDL_STATIC ON CACHE BOOL "Build SDL3 as static library" FORCE)
            set(SDL_TEST OFF CACHE BOOL "Build SDL3 test programs" FORCE)
            set(SDL_TESTS OFF CACHE BOOL "Build SDL3 tests" FORCE)
            set(SDL_DISABLE_INSTALL ON CACHE BOOL "Disable SDL3 installation" FORCE)
            set(SDL_DISABLE_INSTALL_DOCS ON CACHE BOOL "Disable SDL3 documentation installation" FORCE)

            # Disable all subsystems we don't need
            set(SDL_AUDIO OFF CACHE BOOL "Enable SDL3 audio subsystem" FORCE)
            set(SDL_SENSOR OFF CACHE BOOL "Enable SDL3 sensor subsystem" FORCE)
            set(SDL_CAMERA OFF CACHE BOOL "Enable SDL3 camera subsystem" FORCE)
            set(SDL_HAPTIC OFF CACHE BOOL "Enable SDL3 haptic (force feedback) subsystem" FORCE)
            set(SDL_HIDAPI OFF CACHE BOOL "Enable SDL3 HIDAPI subsystem" FORCE)
            set(SDL_POWER OFF CACHE BOOL "Enable SDL3 power subsystem" FORCE)
            set(SDL_MISC OFF CACHE BOOL "Enable SDL3 misc subsystem" FORCE)
            set(SDL_DIALOG OFF CACHE BOOL "Enable SDL3 dialog subsystem" FORCE)
            set(SDL_JOYSTICK OFF CACHE BOOL "Enable SDL3 joystick subsystem" FORCE)
            set(SDL_GAMEPAD OFF CACHE BOOL "Enable SDL3 gamepad subsystem" FORCE)

            # Disable rendering - we only need Vulkan surface
            set(SDL_RENDER OFF CACHE BOOL "Disable SDL3 software/hardware renderer" FORCE)
            set(SDL_GPU OFF CACHE BOOL "Disable SDL3 GPU API (we use raw Vulkan)" FORCE)

            # Enable only what we need
            set(SDL_VIDEO ON CACHE BOOL "Enable SDL3 video subsystem (required for windows)" FORCE)
            set(SDL_EVENTS ON CACHE BOOL "Enable SDL3 events (required for input)" FORCE)
            set(SDL_VULKAN ON CACHE BOOL "Enable SDL3 Vulkan surface creation" FORCE)

            # Disable examples and utilities
            set(SDL_INSTALL_TESTS OFF CACHE BOOL "Install SDL3 test programs" FORCE)
            set(SDL_DISABLE_ANDROID_JAR ON CACHE BOOL "Disable Android JAR" FORCE)
            set(SDL_DISABLE_UNINSTALL ON CACHE BOOL "Disable uninstall target" FORCE)

            add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/SDL3" SDL3 EXCLUDE_FROM_ALL)

        elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SDL3/CMakeLists.txt")
            # Strategy 3: Look in current source directory
            message(STATUS "SDL3 found in ${CMAKE_CURRENT_SOURCE_DIR}/SDL3")

            # Apply same minimal configuration
            set(SDL_SHARED OFF CACHE BOOL "Build SDL3 as shared library" FORCE)
            set(SDL_STATIC ON CACHE BOOL "Build SDL3 as static library" FORCE)
            set(SDL_TEST OFF CACHE BOOL "Build SDL3 test programs" FORCE)
            set(SDL_TESTS OFF CACHE BOOL "Build SDL3 tests" FORCE)
            set(SDL_DISABLE_INSTALL ON CACHE BOOL "Disable SDL3 installation" FORCE)
            set(SDL_DISABLE_INSTALL_DOCS ON CACHE BOOL "Disable SDL3 documentation installation" FORCE)
            set(SDL_AUDIO OFF CACHE BOOL "Enable SDL3 audio subsystem" FORCE)
            set(SDL_SENSOR OFF CACHE BOOL "Enable SDL3 sensor subsystem" FORCE)
            set(SDL_CAMERA OFF CACHE BOOL "Enable SDL3 camera subsystem" FORCE)
            set(SDL_HAPTIC OFF CACHE BOOL "Enable SDL3 haptic (force feedback) subsystem" FORCE)
            set(SDL_HIDAPI OFF CACHE BOOL "Enable SDL3 HIDAPI subsystem" FORCE)
            set(SDL_POWER OFF CACHE BOOL "Enable SDL3 power subsystem" FORCE)
            set(SDL_MISC OFF CACHE BOOL "Enable SDL3 misc subsystem" FORCE)
            set(SDL_DIALOG OFF CACHE BOOL "Enable SDL3 dialog subsystem" FORCE)
            set(SDL_JOYSTICK OFF CACHE BOOL "Enable SDL3 joystick subsystem" FORCE)
            set(SDL_GAMEPAD OFF CACHE BOOL "Enable SDL3 gamepad subsystem" FORCE)
            set(SDL_RENDER OFF CACHE BOOL "Disable SDL3 software/hardware renderer" FORCE)
            set(SDL_GPU OFF CACHE BOOL "Disable SDL3 GPU API (we use raw Vulkan)" FORCE)
            set(SDL_VIDEO ON CACHE BOOL "Enable SDL3 video subsystem (required for windows)" FORCE)
            set(SDL_EVENTS ON CACHE BOOL "Enable SDL3 events (required for input)" FORCE)
            set(SDL_VULKAN ON CACHE BOOL "Enable SDL3 Vulkan surface creation" FORCE)
            set(SDL_INSTALL_TESTS OFF CACHE BOOL "Install SDL3 test programs" FORCE)
            set(SDL_DISABLE_ANDROID_JAR ON CACHE BOOL "Disable Android JAR" FORCE)
            set(SDL_DISABLE_UNINSTALL ON CACHE BOOL "Disable uninstall target" FORCE)

            add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/SDL3" SDL3 EXCLUDE_FROM_ALL)

        else()
            # Strategy 4: Download via FetchContent
            message(STATUS "SDL3 not found locally, downloading from GitHub...")

            include(FetchContent)

            # Configure SDL3 build options for minimal size
            set(SDL_SHARED OFF CACHE BOOL "Build SDL3 as shared library" FORCE)
            set(SDL_STATIC ON CACHE BOOL "Build SDL3 as static library" FORCE)
            set(SDL_TEST OFF CACHE BOOL "Build SDL3 test programs" FORCE)
            set(SDL_TESTS OFF CACHE BOOL "Build SDL3 tests" FORCE)
            set(SDL_DISABLE_INSTALL ON CACHE BOOL "Disable SDL3 installation" FORCE)
            set(SDL_DISABLE_INSTALL_DOCS ON CACHE BOOL "Disable SDL3 documentation installation" FORCE)
            set(SDL_AUDIO OFF CACHE BOOL "Enable SDL3 audio subsystem" FORCE)
            set(SDL_SENSOR OFF CACHE BOOL "Enable SDL3 sensor subsystem" FORCE)
            set(SDL_CAMERA OFF CACHE BOOL "Enable SDL3 camera subsystem" FORCE)
            set(SDL_HAPTIC OFF CACHE BOOL "Enable SDL3 haptic (force feedback) subsystem" FORCE)
            set(SDL_HIDAPI OFF CACHE BOOL "Enable SDL3 HIDAPI subsystem" FORCE)
            set(SDL_POWER OFF CACHE BOOL "Enable SDL3 power subsystem" FORCE)
            set(SDL_MISC OFF CACHE BOOL "Enable SDL3 misc subsystem" FORCE)
            set(SDL_DIALOG OFF CACHE BOOL "Enable SDL3 dialog subsystem" FORCE)
            set(SDL_JOYSTICK OFF CACHE BOOL "Enable SDL3 joystick subsystem" FORCE)
            set(SDL_GAMEPAD OFF CACHE BOOL "Enable SDL3 gamepad subsystem" FORCE)
            set(SDL_RENDER OFF CACHE BOOL "Disable SDL3 software/hardware renderer" FORCE)
            set(SDL_GPU OFF CACHE BOOL "Disable SDL3 GPU API (we use raw Vulkan)" FORCE)
            set(SDL_VIDEO ON CACHE BOOL "Enable SDL3 video subsystem (required for windows)" FORCE)
            set(SDL_EVENTS ON CACHE BOOL "Enable SDL3 events (required for input)" FORCE)
            set(SDL_VULKAN ON CACHE BOOL "Enable SDL3 Vulkan surface creation" FORCE)
            set(SDL_INSTALL_TESTS OFF CACHE BOOL "Install SDL3 test programs" FORCE)
            set(SDL_DISABLE_ANDROID_JAR ON CACHE BOOL "Disable Android JAR" FORCE)
            set(SDL_DISABLE_UNINSTALL ON CACHE BOOL "Disable uninstall target" FORCE)

            FetchContent_Declare(
                SDL3
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG        release-3.4.0
                GIT_SHALLOW    TRUE
                GIT_PROGRESS   TRUE
            )
            FetchContent_MakeAvailable(SDL3)

            message(STATUS "SDL3 downloaded and configured")
        endif()

        # Verify target was created
        if(NOT TARGET SDL3::SDL3)
            message(FATAL_ERROR "SDL3 setup failed - SDL3::SDL3 target not created")
        endif()
    else()
        message(STATUS "SDL3 already in the project")
    endif()

    # Link SDL3 to the target
    target_link_libraries(${target_name} PUBLIC SDL3::SDL3)

    message(STATUS "SDL3 linked to ${target_name}")
endfunction()
