if (NOT DEFINED BALBINO_DIR)
    message(FATAL_ERROR "BALBINO_DIR is not defined.")
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (NOT DEFINED PROJECT_DIR)
    set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")
endif ()

set(CMAKE_CXX_STANDARD 23)

if (NOT CYGWIN)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif ()
