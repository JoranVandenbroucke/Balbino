function(setup_sdl3 target_name)
    set(SDL_SHARED ON CACHE BOOL "Build SDL shared library")
    set(SDL_STATIC ON CACHE BOOL "Build SDL static library")
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL test library")
    set(SDL_TESTS OFF CACHE BOOL "Build SDL test programs")
    set(SDL_DISABLE_INSTALL ON CACHE BOOL "Don't create an SDL install target")
    set(SDL_DISABLE_INSTALL_DOCS ON CACHE BOOL "Don't install the SDL documentation")
    set(SDL_INSTALL_TESTS OFF CACHE BOOL "Install the SDL test programs")

    set(SDL_AUDIO ON CACHE BOOL "Enable the Audio Subsystem")
    set(SDL_VIDEO ON CACHE BOOL "Enable the Video Subsystem")
    set(SDL_GPU ON CACHE BOOL "Enable the GPU Subsystem")
    set(SDL_RENDER OFF CACHE BOOL "Enable the Render Subsystem")
    set(SDL_CAMERA OFF CACHE BOOL "Enable the Camera Subsystem")
    set(SDL_JOYSTICK OFF CACHE BOOL "Enable the Joystick Subsystem")
    set(SDL_HAPTIC OFF CACHE BOOL "Enable the Haptic Subsystem")
    set(SDL_HIDAPI OFF CACHE BOOL "Enable the Hidapi Subsystem")
    set(SDL_POWER OFF CACHE BOOL "Enable the Sensor Subsystem")
    set(SDL_SENSOR OFF CACHE BOOL "Enable the Sensor Subsystem")
    set(SDL_DIALOG OFF CACHE BOOL "Enable the Dialog Subsystem")

    # Check if SDL3 is already a target
    if (NOT TARGET SDL3::SDL3)
        # Try to find SDL3 package
        find_package(SDL3 QUIET PATHS "${CMAKE_SOURCE_DIR}/3rdParty/SDL")

        # If SDL3 is not found, attempt to add it from source or fetch it
        if (NOT SDL3_FOUND)
            if (EXISTS "${CMAKE_SOURCE_DIR}/3rdParty/SDL/CMakeLists.txt")
                add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/SDL" SDL EXCLUDE_FROM_ALL)
            elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SDL/CMakeLists.txt")
                add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/SDL" SDL EXCLUDE_FROM_ALL)
            else ()
                include(FetchContent)


                FetchContent_Declare(
                        SDL
                        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                        GIT_TAG main  # Replace with a specific tag or commit hash if needed
                        GIT_SHALLOW TRUE
                        GIT_PROGRESS TRUE
                )
                FetchContent_MakeAvailable(SDL)
                set_property(DIRECTORY "${CMAKE_BINARY_DIR}/_deps/sdl-src" PROPERTY EXCLUDE_FROM_ALL TRUE)
            endif ()
        endif ()
    endif ()

    # Add SDL3 as a dependency and link it to the target
    add_dependencies(${target_name} SDL3::SDL3)
    target_include_directories(${target_name} PUBLIC ${SDL3_INCLUDE_DIRS})
    target_link_libraries(${target_name} PUBLIC SDL3::SDL3)
endfunction()
