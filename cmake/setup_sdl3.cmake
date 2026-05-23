function(setup_sdl3 target_name)
    # Predeclare SDL3_SOURCE_DIR
    set(SDL3_SOURCE_DIR "SDL3::SDL3")
    # Check if SDL3 is already a target
    if (NOT TARGET SDL3::SDL3)
        message(STATUS "Finding SDL3")
        # Example for SDL3:
        set(SDL_DIRECTX OFF CACHE BOOL "" FORCE)
        set(SDL_VULKAN OFF CACHE BOOL "" FORCE)
        set(SDL_RENDER OFF CACHE BOOL "" FORCE)    # if you want to disable SDL's internal software renderer
        set(SDL_OPENGL ON CACHE BOOL "" FORCE)
        set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
        set(SDL_TESTS OFF CACHE BOOL "" FORCE)
        set(SDL_DISABLE_INSTALL OFF CACHE BOOL "" FORCE)
        set(SDL_DISABLE_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
        set(SDL_INSTALL_TESTS OFF CACHE BOOL "" FORCE)

        # Try to find SDL3 package
        find_package(SDL3)
        if( SDL3_FOUND )
            message(STATUS "SDL3 Found")
            target_link_libraries(${target_name} PUBLIC SDL3::SDL3)
        else()
            # If SDL3 is not found, attempt to add it from source or fetch it
            if (EXISTS "${CMAKE_SOURCE_DIR}/3rdParty/SDL3/CMakeLists.txt")
                message("In Source SDL3")
                add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/SDL3" SDL3 EXCLUDE_FROM_ALL)
                set(SDL3_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdParty/SDL3")
            elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SDL3/CMakeLists.txt")
                message(STATUS "In Subdir SDL3")
                add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/SDL3" SDL3 EXCLUDE_FROM_ALL)
                set(SDL3_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL3")
            else ()
                include(FetchContent)
                message(STATUS "Fetching Dear SDL3...")
                FetchContent_Declare(
                    SDL3
                    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                    GIT_TAG main
                    GIT_SHALLOW    TRUE
                    GIT_PROGRESS   TRUE
                )
                FetchContent_MakeAvailable(SDL3)
            endif ()
            get_target_property(SDL3_INCLUDE_DIRS SDL3::SDL3 INCLUDE_DIRECTORIES)
            add_dependencies(${target_name} SDL3::SDL3)
            target_include_directories(${target_name} PUBLIC ${SDL3_INCLUDE_DIRS})
            target_link_libraries(${target_name} PUBLIC SDL3::SDL3)
        endif ()
    else ()
        get_target_property(SDL3_SOURCE_DIR SDL3::SDL3 SOURCE_DIR)
        get_target_property(SDL3_INCLUDE_DIRS SDL3::SDL3 INCLUDE_DIRECTORIES)
        set(SDL3_SOURCE_DIR ${SDL3_SOURCE_DIR}/..)
        add_dependencies(${target_name} SDL3::SDL3)
        target_include_directories(${target_name} PUBLIC ${SDL3_INCLUDE_DIRS})
        target_link_libraries(${target_name} PUBLIC SDL3::SDL3)
    endif ()
endfunction()
