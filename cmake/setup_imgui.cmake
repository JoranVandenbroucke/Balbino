function(setup_imgui target_name)
    include(FetchContent)

    # --- ImImGui ---
    if (NOT TARGET imgui)
        message(STATUS "Fetching Dear ImImGui...")
        FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/ImGui.git
            GIT_TAG docking
            GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(imgui)

        # Create an ImImGui static library
        target_sources(${target_name} PRIVATE
                PRIVATE
                ${imgui_SOURCE_DIR}/imgui.cpp
                ${imgui_SOURCE_DIR}/imgui_demo.cpp
                ${imgui_SOURCE_DIR}/imgui_draw.cpp
                ${imgui_SOURCE_DIR}/imgui_tables.cpp
                ${imgui_SOURCE_DIR}/imgui_widgets.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
                PRIVATE
                FILE_SET imgui_headers TYPE HEADERS
                BASE_DIRS
                ${imgui_SOURCE_DIR}
                FILES
                ${imgui_SOURCE_DIR}/imconfig.h
                ${imgui_SOURCE_DIR}/imgui.h
                ${imgui_SOURCE_DIR}/imgui_internal.h
                ${imgui_SOURCE_DIR}/imstb_rectpack.h
                ${imgui_SOURCE_DIR}/imstb_textedit.h
                ${imgui_SOURCE_DIR}/imstb_truetype.h
                ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
                ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        )

        target_include_directories(${target_name} PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
        )

        target_compile_definitions(${target_name} PRIVATE "IMImGui_IMPL_OPENGL_LOADER_GLAD")
    endif ()
endfunction()
