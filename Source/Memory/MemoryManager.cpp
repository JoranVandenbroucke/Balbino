//
// Created by joran on 16/08/2023.
//
#include "MemoryManager.h"

namespace FawnMemAlloc
{
    void* MemoryManager::Allocate( size_t n, std::source_location location )
    {
        return m_allocator.Alloc( n, location ).ptr;
    }
    void MemoryManager::Free( void* ptr )
    {
        if ( m_allocator.Owns( { ptr, 0 } ) )
            m_allocator.Free( { ptr, 0 } );
    }
}
    void* operator new( std::size_t count )
    {
        return FawnMemAlloc::g_memoryManager.Allocate( count );
    }
    void* operator new[]( std::size_t count )
    {
        return FawnMemAlloc::g_memoryManager.Allocate( count );
    }
    void operator delete( void* ptr )
    {
        FawnMemAlloc::g_memoryManager.Free( ptr );
    }
    void operator delete[]( void* ptr )
    {
        FawnMemAlloc::g_memoryManager.Free( ptr );
    }