//
// Created by joran on 16/08/2023.
//

#pragma once
#include "../MemoryManagerCore.h"

namespace FawnMemAlloc
{
    template<size_t s>
    class StackAllocator
    {
    public:
        Blk Alloc( size_t n );
        void Free( Blk b );
        bool Owns( Blk b );
        void FreeAll();

    private:
        char m_memoryStack[ s ];
        char* m_head;
        size_t RoundToAlign( size_t size );
    };
    template<size_t s>
    Blk StackAllocator<s>::Alloc( size_t n )
    {
        size_t n1 = RoundToAlign( n );
        if ( n1 > size_t( ( m_memoryStack + s ) - m_head ) )
        {
            return { nullptr, 0 };
        }
        Blk result { m_head, n };
        m_head += n1;
        return result;
    }
    template<size_t s>
    void StackAllocator<s>::Free( Blk b )
    {
        if ( reinterpret_cast<char*>( b.ptr ) + RoundToAlign( b.size ) == m_head )
            m_head = reinterpret_cast<char*>( b.ptr );
    }
    template<size_t s>
    bool StackAllocator<s>::Owns( Blk b )
    {
        return b.ptr >= m_memoryStack && b.ptr < m_memoryStack + s;
    }
    template<size_t s>
    void StackAllocator<s>::FreeAll()
    {
        m_head = m_memoryStack;
    }
    template<size_t s>
    size_t StackAllocator<s>::RoundToAlign( size_t size )
    {
        int remainder = size % sizeof( float );
        if ( remainder == 0 )
            return size;

        return size + sizeof( float ) - remainder;
    }
}