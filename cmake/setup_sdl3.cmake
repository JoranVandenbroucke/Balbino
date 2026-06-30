function(setup_sdl3 target_name)
    # ------------------------------------------------------------------
    # Helper: wire up the post-build copy + include/link for any path
    # ------------------------------------------------------------------
    macro(_sdl3_link_and_copy)
        target_link_libraries(${target_name} PUBLIC SDL3::SDL3)

        # Install: copy next to exe in the install tree
        install(FILES $<TARGET_FILE:SDL3::SDL3>
                DESTINATION bin
        )
        add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL3::SDL3>
                $<TARGET_FILE_DIR:${target_name}>
        )
    endmacro()

    if (TARGET SDL3::SDL3)
        # Already defined upstream — just link and copy
        _sdl3_link_and_copy()
        return()
    endif ()

    message(STATUS "SDL3 not found as target, searching...")

    # SDL3 build options
    set(SDL_DIRECTX OFF CACHE BOOL "" FORCE)
    set(SDL_VULKAN ON CACHE BOOL "" FORCE)
    set(SDL_RENDER OFF CACHE BOOL "" FORCE)
    set(SDL_OPENGL ON CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)
    set(SDL_DISABLE_INSTALL OFF CACHE BOOL "" FORCE)
    set(SDL_DISABLE_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
    set(SDL_INSTALL_TESTS OFF CACHE BOOL "" FORCE)

    find_package(SDL3 QUIET)
    if (SDL3_FOUND)
        message(STATUS "SDL3 found via find_package")
        _sdl3_link_and_copy()
        return()
    endif ()

    # Source fallbacks
    if (EXISTS "${CMAKE_SOURCE_DIR}/3rdParty/SDL3/CMakeLists.txt")
        message(STATUS "SDL3: using 3rdParty/SDL3")
        add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/SDL3" SDL3 EXCLUDE_FROM_ALL)
    elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SDL3/CMakeLists.txt")
        message(STATUS "SDL3: using local SDL3 subdirectory")
        add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/SDL3" SDL3 EXCLUDE_FROM_ALL)
    else ()
        message(STATUS "SDL3: fetching from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
                SDL3
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG main
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(SDL3)
    endif ()

    _sdl3_link_and_copy()
endfunction()