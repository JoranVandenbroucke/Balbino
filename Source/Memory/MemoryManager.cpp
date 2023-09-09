//
// Created by joran on 16/08/2023.
//

#ifdef USE_MEMORY_MANAGER
#include "MemoryManager.h"

namespace FawnMemAlloc
{
#ifdef _DEBUG
    void* MemoryManager::Allocate( std::size_t n, std::source_location location )
    {
        return m_allocator.Alloc( n, location ).ptr;
    }
#else
    void* MemoryManager::Allocate( std::size_t n )
    {
        return m_allocator.Alloc( n ).ptr;
    }
#endif
    void MemoryManager::Free( void* ptr, std::size_t size )
    {
        if ( m_allocator.Owns( { ptr, size } ) )// todo: find out how to properly use this so that size is not 0
            m_allocator.Free( { ptr, size } );  // todo: find out how to properly use this so that size is not 0
    }
}// namespace FawnMemAlloc
#ifdef _DEBUG
void* operator new( std::size_t count, std::source_location location )
{
    return FawnMemAlloc::g_memoryManager.Allocate( count, location );
}
void* operator new[]( std::size_t count, std::source_location location )
{
    return FawnMemAlloc::g_memoryManager.Allocate( count, location );
}
#else

void* operator new( std::size_t count )
{
    return FawnMemAlloc::g_memoryManager.Allocate( count );
}
void* operator new[]( std::size_t count )
{
    return FawnMemAlloc::g_memoryManager.Allocate( count );
}
#endif
void operator delete( void* ptr )
{
    FawnMemAlloc::g_memoryManager.Free( ptr );
}
void operator delete[]( void* ptr )
{
    FawnMemAlloc::g_memoryManager.Free( ptr );
}

void operator delete( void* ptr, std::size_t size )
{
    FawnMemAlloc::g_memoryManager.Free( ptr, size );
}
void operator delete[]( void* ptr, std::size_t size )
{
    FawnMemAlloc::g_memoryManager.Free( ptr, size );
}
#endif