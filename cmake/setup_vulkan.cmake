function(setup_vulkan target_name)
    if(NOT TARGET Vulkan::Vulkan)

        # --- Candidate search paths (ordered by priority) ---
        set(_VULKAN_SEARCH_PATHS "")

        # 1. 3rd_party / middleware local vendor dirs
        foreach(_VENDOR_ROOT
            "${CMAKE_SOURCE_DIR}/3rd_party"
            "${CMAKE_SOURCE_DIR}/3rdparty"
            "${CMAKE_SOURCE_DIR}/third_party"
            "${CMAKE_SOURCE_DIR}/middleware"
            "${CMAKE_SOURCE_DIR}/extern"
            "${CMAKE_SOURCE_DIR}/external"
        )
            if(EXISTS "${_VENDOR_ROOT}")
                # Common sub-layouts: vulkan/, VulkanSDK/, sdk/vulkan/, etc.
                foreach(_SUB "" "/vulkan" "/VulkanSDK" "/sdk/vulkan" "/Vulkan")
                    list(APPEND _VULKAN_SEARCH_PATHS "${_VENDOR_ROOT}${_SUB}")
                endforeach()
            endif()
        endforeach()

        # 2. Project cmake/ config dirs
        list(APPEND _VULKAN_SEARCH_PATHS
            "${CMAKE_SOURCE_DIR}/cmake"
            "${CMAKE_SOURCE_DIR}/cmake/local"
        )

        # --- Strategy 1: system find_package (honours VULKAN_SDK env var etc.) ---
        find_package(Vulkan QUIET)

        if(Vulkan_FOUND)
            message(STATUS "[Vulkan] Found via system find_package")
            message(STATUS "         include : ${Vulkan_INCLUDE_DIRS}")
            message(STATUS "         library : ${Vulkan_LIBRARIES}")

        else()
            # --- Strategy 2: vendor / cmake config paths ---
            find_package(Vulkan QUIET
                PATHS ${_VULKAN_SEARCH_PATHS}
                NO_DEFAULT_PATH
            )

            if(Vulkan_FOUND)
                message(STATUS "[Vulkan] Found in vendor/cmake paths")
                message(STATUS "         include : ${Vulkan_INCLUDE_DIRS}")
                message(STATUS "         library : ${Vulkan_LIBRARIES}")
            else()
                # --- Strategy 3: walk vendor roots for a VulkanConfig.cmake or
                #     an include/vulkan/vulkan.h + lib/vulkan-1 pair manually   ---
                set(_VULKAN_MANUAL_ROOT "")
                foreach(_ROOT ${_VULKAN_SEARCH_PATHS})
                    if(EXISTS "${_ROOT}/include/vulkan/vulkan.h")
                        set(_VULKAN_MANUAL_ROOT "${_ROOT}")
                        break()
                    endif()
                endforeach()

                if(_VULKAN_MANUAL_ROOT)
                    message(STATUS "[Vulkan] Manually bootstrapping from ${_VULKAN_MANUAL_ROOT}")

                    # Locate the import lib / .so
                    find_library(_VULKAN_MANUAL_LIB
                        NAMES vulkan-1 vulkan
                        PATHS
                            "${_VULKAN_MANUAL_ROOT}/lib"
                            "${_VULKAN_MANUAL_ROOT}/Lib"
                            "${_VULKAN_MANUAL_ROOT}/lib64"
                        NO_DEFAULT_PATH
                    )

                    if(_VULKAN_MANUAL_LIB)
                        # Synthesise the imported target that find_package would have made
                        add_library(Vulkan::Vulkan SHARED IMPORTED GLOBAL)
                        set_target_properties(Vulkan::Vulkan PROPERTIES
                            IMPORTED_LOCATION             "${_VULKAN_MANUAL_LIB}"
                            INTERFACE_INCLUDE_DIRECTORIES "${_VULKAN_MANUAL_ROOT}/include"
                        )
                        # Mimic the cache vars downstream code might query
                        set(Vulkan_FOUND        TRUE  CACHE BOOL   "" FORCE)
                        set(Vulkan_INCLUDE_DIRS "${_VULKAN_MANUAL_ROOT}/include" CACHE PATH "" FORCE)
                        set(Vulkan_LIBRARIES    "${_VULKAN_MANUAL_LIB}"          CACHE PATH "" FORCE)
                        message(STATUS "         include : ${_VULKAN_MANUAL_ROOT}/include")
                        message(STATUS "         library : ${_VULKAN_MANUAL_LIB}")
                    else()
                        set(_VULKAN_MANUAL_ROOT "") # header found but no lib – keep searching
                    endif()
                endif()

                if(NOT Vulkan_FOUND AND NOT TARGET Vulkan::Vulkan)
                    # Build a readable list of everything we tried for the error
                    list(JOIN _VULKAN_SEARCH_PATHS "\n    " _TRIED_PATHS_STR)
                    message(FATAL_ERROR
                        "[Vulkan] SDK not found!\n"
                        "Tried (in order):\n"
                        "  1. System paths / VULKAN_SDK env var (find_package)\n"
                        "  2. Vendor/cmake config paths:\n"
                        "       ${_TRIED_PATHS_STR}\n"
                        "  3. Header+lib scan of the above roots\n\n"
                        "Fix options:\n"
                        "  • Install the LunarG Vulkan SDK and set VULKAN_SDK in your environment\n"
                        "  • Drop the SDK under one of the vendor roots above\n"
                        "    (needs include/vulkan/vulkan.h and lib/vulkan-1)\n"
                        "  • Place a VulkanConfig.cmake in cmake/ or cmake/local/\n"
                    )
                endif()
            endif()
        endif()

        # Sanity-check: find_package succeeded but didn't create the target
        # (can happen with very old CMake + Vulkan SDK combos)
        if(NOT TARGET Vulkan::Vulkan)
            message(FATAL_ERROR
                "[Vulkan] find_package reported success but Vulkan::Vulkan target "
                "was not created. CMake >= 3.21 and Vulkan SDK >= 1.2 are required."
            )
        endif()

    else()
        message(STATUS "[Vulkan] Target Vulkan::Vulkan already exists, skipping discovery")
    endif()

    # -------------------------------------------------------------------------
    # Link
    # -------------------------------------------------------------------------
    target_link_libraries(${target_name} PUBLIC Vulkan::Vulkan)

    # -------------------------------------------------------------------------
    # Windows: copy vulkan-1.dll next to the executable
    # -------------------------------------------------------------------------
    if(WIN32)
        # Resolve the lib path from whichever strategy succeeded
        set(_VK_LIB "")
        if(Vulkan_LIBRARY)
            set(_VK_LIB "${Vulkan_LIBRARY}")
        else()
            # Walk config types; Release first, then generic IMPORTED_LOCATION
            foreach(_CFG RELEASE RELWITHDEBINFO DEBUG "")
                if(_CFG)
                    get_target_property(_VK_LIB Vulkan::Vulkan "IMPORTED_LOCATION_${_CFG}")
                else()
                    get_target_property(_VK_LIB Vulkan::Vulkan IMPORTED_LOCATION)
                endif()
                if(_VK_LIB)
                    break()
                endif()
            endforeach()
        endif()

        if(_VK_LIB)
            get_filename_component(_VK_LIB_DIR "${_VK_LIB}" DIRECTORY)

            # Probe common SDK layouts for the DLL
            set(_DLL_CANDIDATES
                "${_VK_LIB_DIR}/vulkan-1.dll"           # lib/ == bin/ (rare)
                "${_VK_LIB_DIR}/../Bin/vulkan-1.dll"    # LunarG layout  x64
                "${_VK_LIB_DIR}/../bin/vulkan-1.dll"    # lowercase variant
                "${_VK_LIB_DIR}/../../Bin/vulkan-1.dll" # nested SDK layout
            )

            set(_VK_DLL "")
            foreach(_CANDIDATE ${_DLL_CANDIDATES})
                cmake_path(NORMAL_PATH _CANDIDATE OUTPUT_VARIABLE _CANDIDATE_NORM)
                if(EXISTS "${_CANDIDATE_NORM}")
                    set(_VK_DLL "${_CANDIDATE_NORM}")
                    break()
                endif()
            endforeach()

            if(_VK_DLL)
                add_custom_command(TARGET ${target_name} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${_VK_DLL}"
                        "$<TARGET_FILE_DIR:${target_name}>"
                    COMMENT "[Vulkan] Copying vulkan-1.dll -> $<TARGET_FILE_DIR:${target_name}>"
                    VERBATIM
                )
                message(STATUS "[Vulkan] DLL staged for copy: ${_VK_DLL}")
            else()
                message(WARNING
                    "[Vulkan] Could not locate vulkan-1.dll near ${_VK_LIB_DIR}.\n"
                    "         The runtime DLL will not be copied automatically.\n"
                    "         Add it to your PATH or copy it manually."
                )
            endif()
        endif()
    endif()
endfunction()
