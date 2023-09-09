//
// Created by joran on 16/08/2023.
//

#pragma once
#include "../MemoryManagerCore.h"

namespace FawnMemAlloc
{
    template<class A, std::size_t min, std::size_t max, std::size_t group, std::size_t total>
    class FreelistAllocator
    {
    public:
        Blk Alloc( std::size_t n );
        void Free( Blk b );
        bool Owns( Blk b );

    private:
        struct Node {
            Node* next { nullptr };
        };
        A m_parent {};
        Node m_root {};
        std::size_t m_count {};
    };

    template<class A, std::size_t min, std::size_t max, std::size_t group, std::size_t total>
    Blk FreelistAllocator<A, min, max, group, total>::Alloc( std::size_t n )
    {
        if ( n >= min && n <= max && m_root.next && m_count + group <= total )
        {
            Blk b { reinterpret_cast<void*>( &m_root ), max * group };
            for ( std::size_t i = 0; i < group; ++i )
            {
                m_root = *m_root.next;
            }
            m_count += group;
            return b;
        }
        return m_parent.Alloc( n );
    }
    template<class A, std::size_t min, std::size_t max, std::size_t group, std::size_t total>
    void FreelistAllocator<A, min, max, group, total>::Free( Blk b )
    {
        if ( m_parent.Owns( b ) )
        {
            m_root.next = reinterpret_cast<Node*>( b.ptr );
            ++m_count;
            return m_parent.Free( b );
        }
        // Deallocate memory in batch
        for ( std::size_t i = 0; i < group && m_count; ++i )
        {
            auto p  = reinterpret_cast<Node*>( b.ptr ) + i;
            p->next = &m_root;
            --m_count;
            m_root = *p;
        }
    }
    template<class A, std::size_t min, std::size_t max, std::size_t group, std::size_t total>
    bool FreelistAllocator<A, min, max, group, total>::Owns( Blk b )
    {
        return ( b.size >= min && b.size <= max ) || m_parent.Owns( b );
    }
}// namespace FawnMemAlloc