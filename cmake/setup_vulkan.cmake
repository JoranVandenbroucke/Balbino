function(setup_vulkan target_name)
    if(NOT TARGET Vulkan::cppm)
        set(ENABLE_CPP20_MODULE ON CACHE BOOL "" FORCE)

        # --- Candidate search paths (ordered by priority) ---
        set(_VULKAN_SEARCH_PATHS "")

        foreach(_VENDOR_ROOT
            "${CMAKE_SOURCE_DIR}/3rd_party"
            "${CMAKE_SOURCE_DIR}/3rdparty"
            "${CMAKE_SOURCE_DIR}/third_party"
            "${CMAKE_SOURCE_DIR}/middleware"
            "${CMAKE_SOURCE_DIR}/extern"
            "${CMAKE_SOURCE_DIR}/external"
        )
            if(EXISTS "${_VENDOR_ROOT}")
                foreach(_SUB "" "/vulkan" "/VulkanSDK" "/sdk/vulkan" "/Vulkan")
                    list(APPEND _VULKAN_SEARCH_PATHS "${_VENDOR_ROOT}${_SUB}")
                endforeach()
            endif()
        endforeach()

        list(APPEND _VULKAN_SEARCH_PATHS
            "${CMAKE_SOURCE_DIR}/cmake"
            "${CMAKE_SOURCE_DIR}/cmake/local"
        )

        # --- Strategy 1: system find_package ---
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
                # --- Strategy 3: manual header+lib scan ---
                set(_VULKAN_MANUAL_ROOT "")
                foreach(_ROOT ${_VULKAN_SEARCH_PATHS})
                    if(EXISTS "${_ROOT}/include/vulkan/vulkan.h")
                        set(_VULKAN_MANUAL_ROOT "${_ROOT}")
                        break()
                    endif()
                endforeach()

                if(_VULKAN_MANUAL_ROOT)
                    message(STATUS "[Vulkan] Manually bootstrapping from ${_VULKAN_MANUAL_ROOT}")

                    find_library(_VULKAN_MANUAL_LIB
                        NAMES vulkan-1 vulkan
                        PATHS
                            "${_VULKAN_MANUAL_ROOT}/lib"
                            "${_VULKAN_MANUAL_ROOT}/Lib"
                            "${_VULKAN_MANUAL_ROOT}/lib64"
                        NO_DEFAULT_PATH
                    )

                    if(_VULKAN_MANUAL_LIB)
                        # FIX: Create Vulkan::Vulkan (the real import target), NOT Vulkan::cppm.
                        # The module block below creates the cppm alias after VulkanCppModule is set up.
                        add_library(Vulkan::Vulkan SHARED IMPORTED GLOBAL)
                        set_target_properties(Vulkan::Vulkan PROPERTIES
                            IMPORTED_LOCATION             "${_VULKAN_MANUAL_LIB}"
                            INTERFACE_INCLUDE_DIRECTORIES "${_VULKAN_MANUAL_ROOT}/include"
                        )
                        set(Vulkan_FOUND        TRUE                             CACHE BOOL "" FORCE)
                        set(Vulkan_INCLUDE_DIRS "${_VULKAN_MANUAL_ROOT}/include" CACHE PATH "" FORCE)
                        # FIX: also set singular form used by vulkan.cppm path lookup below
                        set(Vulkan_INCLUDE_DIR  "${_VULKAN_MANUAL_ROOT}/include" CACHE PATH "" FORCE)
                        set(Vulkan_LIBRARIES    "${_VULKAN_MANUAL_LIB}"          CACHE PATH "" FORCE)
                        message(STATUS "         include : ${_VULKAN_MANUAL_ROOT}/include")
                        message(STATUS "         library : ${_VULKAN_MANUAL_LIB}")
                    else()
                        set(_VULKAN_MANUAL_ROOT "")
                    endif()
                endif()

                if(NOT Vulkan_FOUND)
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

        # --- Normalise Vulkan_INCLUDE_DIR (singular) from DIRS (plural) if needed ---
        # find_package(Vulkan) sets both, but the manual path above or old SDK versions
        # may only set one. The .cppm path lookup requires the singular form.
        if(NOT Vulkan_INCLUDE_DIR AND Vulkan_INCLUDE_DIRS)
            list(GET Vulkan_INCLUDE_DIRS 0 Vulkan_INCLUDE_DIR)
            set(Vulkan_INCLUDE_DIR "${Vulkan_INCLUDE_DIR}" CACHE PATH "" FORCE)
        endif()

        # --- C++20 module setup ---
        if(ENABLE_CPP20_MODULE)
            if(NOT TARGET VulkanCppModule)

                # Verify the .cppm source exists before trying to compile it
                set(_VULKAN_CPPM "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm")
                if(NOT EXISTS "${_VULKAN_CPPM}")
                    message(FATAL_ERROR
                        "[Vulkan] C++20 module requested (ENABLE_CPP20_MODULE=ON) but "
                        "vulkan.cppm was not found at:\n  ${_VULKAN_CPPM}\n"
                        "Vulkan SDK >= 1.3.261 is required for C++20 module support.\n"
                        "Disable with -DENABLE_CPP20_MODULE=OFF to use header-only mode."
                    )
                endif()

                add_library(VulkanCppModule)
                # FIX: alias created AFTER the library target exists
                add_library(Vulkan::cppm ALIAS VulkanCppModule)

                target_compile_definitions(VulkanCppModule PUBLIC
                    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
                    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
                    # FIX: advertise module mode to consuming targets
                    USE_CPP20_MODULES=1
                )

                # PRIVATE: only needed to compile vulkan.cppm itself, not by consumers
                target_include_directories(VulkanCppModule PRIVATE
                    "${Vulkan_INCLUDE_DIR}"
                )

                # FIX: link to Vulkan::Vulkan (the real imported lib), not the self-alias
                target_link_libraries(VulkanCppModule PUBLIC
                    Vulkan::Vulkan
                )

                set_target_properties(VulkanCppModule PROPERTIES
                    CXX_STANDARD          20
                    CXX_STANDARD_REQUIRED ON
                )

                target_sources(VulkanCppModule
                    PUBLIC
                    FILE_SET  cxx_modules
                    TYPE      CXX_MODULES
                    BASE_DIRS "${Vulkan_INCLUDE_DIR}"
                    FILES     "${_VULKAN_CPPM}"
                )

                message(STATUS "[Vulkan] C++20 module enabled: ${_VULKAN_CPPM}")

            endif()
        else()
            # Module mode off: synthesise a plain pass-through alias so downstream
            # target_link_libraries(foo Vulkan::cppm) still works either way.
            if(NOT TARGET VulkanCppModule)
                add_library(VulkanCppModule INTERFACE)
                add_library(Vulkan::cppm ALIAS VulkanCppModule)
                target_link_libraries(VulkanCppModule INTERFACE Vulkan::Vulkan)
                message(STATUS "[Vulkan] C++20 module disabled, Vulkan::cppm is a header-only passthrough")
            endif()
        endif()

        if(NOT TARGET Vulkan::cppm)
            message(FATAL_ERROR
                "[Vulkan] find_package reported success but Vulkan::cppm target "
                "was not created. CMake >= 3.28 and Vulkan SDK >= 1.3.261 are required "
                "for C++20 module support."
            )
        endif()

    else()
        message(STATUS "[Vulkan] Target Vulkan::cppm already exists, skipping discovery")
    endif()

    # -------------------------------------------------------------------------
    # Link — identical whether modules are on or off
    # -------------------------------------------------------------------------
    target_link_libraries(${target_name} PUBLIC Vulkan::cppm)

    # -------------------------------------------------------------------------
    # Windows: copy vulkan-1.dll next to the executable
    # -------------------------------------------------------------------------
    if(WIN32)
        set(_VK_LIB "")
        if(Vulkan_LIBRARY)
            set(_VK_LIB "${Vulkan_LIBRARY}")
        else()
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

            set(_DLL_CANDIDATES
                "${_VK_LIB_DIR}/vulkan-1.dll"
                "${_VK_LIB_DIR}/../Bin/vulkan-1.dll"
                "${_VK_LIB_DIR}/../bin/vulkan-1.dll"
                "${_VK_LIB_DIR}/../../Bin/vulkan-1.dll"
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
