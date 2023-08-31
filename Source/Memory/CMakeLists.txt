set(PROJECT_NAME FawnMemAlloc)

add_library(${PROJECT_NAME} STATIC
        Allocators/AffixAllocator.h
        Allocators/DoubleBufferAllocator.h
        Allocators/FallbackAllocator.h
        Allocators/FreelistAllocator.h
        Allocators/Mallocator.h
        Allocators/PoolAllocator.h
        Allocators/StackAllocator.h
        MemoryManager.cpp
        MemoryManager.h
        MemoryManagerCore.h
        Allocators/AllocatorWithStats.h
        Allocators/Mallocator.cpp
)

include(${CMAKE_SOURCE_DIR}/CMake/CommonUtils.cmake)
set_precompiled_header(${PROJECT_NAME})
set_compile_options(${PROJECT_NAME} PRIVATE)
set_definitions(${PROJECT_NAME} PRIVATE)
target_compile_definitions(${PROJECT_NAME} PRIVATE "_LIB")

