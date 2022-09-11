project(Common)
file(GLOB headers *.h Components/*.h)
add_library(Common INTERFACE ${headers})

target_compile_definitions(Common INTERFACE
        "BL_PLATFORM_WINDOWS;"
        )