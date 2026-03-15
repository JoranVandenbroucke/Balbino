function(setup_catch target_name)
    if(NOT TARGET Catch2::Catch2)
        find_package(Catch2 QUIET)

        if(Catch2_FOUND)
            message(STATUS "Catch2 found via find_package")
        elseif(EXISTS "${CMAKE_SOURCE_DIR}/3rdParty/Catch2/CMakeLists.txt")
            message(STATUS "Catch2 found in ${CMAKE_SOURCE_DIR}/3rdParty/Catch2")
            add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/Catch2" Catch2 EXCLUDE_FROM_ALL)
        elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Catch2/CMakeLists.txt")
            message(STATUS "Catch2 found in ${CMAKE_CURRENT_SOURCE_DIR}/Catch2")
            add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Catch2" Catch2 EXCLUDE_FROM_ALL)
        else()
            message(STATUS "Catch2 not found locally, downloading from GitHub...")

            include(FetchContent)

            set(BUILD_TESTING OFF CACHE BOOL "Disable Catch2's own tests" FORCE)
            set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Disable Catch2 documentation installation" FORCE)
            set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "Disable Catch2 extras installation" FORCE)
            set(CATCH_DEVELOPMENT_BUILD OFF CACHE BOOL "Disable Catch2 development mode" FORCE)

            FetchContent_Declare(
                Catch2
                GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                GIT_TAG        v3.7.1  # Use a stable release tag instead of devel
                GIT_SHALLOW    TRUE
                GIT_PROGRESS   TRUE
            )
            FetchContent_MakeAvailable(Catch2)

            message(STATUS "Catch2 downloaded and configured")
        endif()

        if(NOT TARGET Catch2::Catch2)
            message(FATAL_ERROR "Catch2 setup failed - Catch2::Catch2 target not created")
        endif()
    else()
        message(STATUS "Catch2 already in the project")
    endif()

    target_link_libraries(${target_name} PRIVATE Catch2::Catch2WithMain)

    if(Catch2_SOURCE_DIR)
        list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
    elseif(TARGET Catch2::Catch2)
        get_target_property(CATCH2_SOURCE_DIR Catch2::Catch2 SOURCE_DIR)
        if(CATCH2_SOURCE_DIR)
            get_filename_component(CATCH2_SOURCE_DIR "${CATCH2_SOURCE_DIR}" DIRECTORY)
            list(APPEND CMAKE_MODULE_PATH "${CATCH2_SOURCE_DIR}/extras")
        endif()
    endif()

    include(CTest)
    include(Catch)
    catch_discover_tests(${target_name})

    message(STATUS "Catch2 linked to ${target_name} and tests configured")
endfunction()
