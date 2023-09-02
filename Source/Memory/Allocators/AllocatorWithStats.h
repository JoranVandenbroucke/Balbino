//
// Created by joran on 18/08/2023.
//

#pragma once
#include "../MemoryManagerCore.h"
#include <cstdint>
#include <ctime>
#include <iostream>
#include <source_location>
#include <unordered_map>

namespace FawnMemAlloc
{
    template<class A, uint64_t flags>
    class AllocatorWithStats
    {
    public:
        AllocatorWithStats() = default;
        ~AllocatorWithStats();
        Blk Alloc( std::size_t s, const std::source_location& location = std::source_location::current() );
        void Free( Blk b );
        bool Owns( Blk b );

    private:
        struct AllocationInfo {
            std::source_location location;
            std::time_t timestamp {};
            std::size_t size {};
        };

        A m_allocator;
        std::unordered_map<void*, AllocationInfo> m_allocationInfoMap;
    };
    template<class A, uint64_t flags>
    AllocatorWithStats<A, flags>::~AllocatorWithStats()
    {
        for ( auto& entry : m_allocationInfoMap )
        {
            std::cout << "Memory created at: " << entry.second.location.file_name() << ":" << entry.second.location.line() << " (" << entry.second.location.function_name() << ") "
                      << "; is not freed." << std::endl;
            m_allocator.Free( { entry.first, entry.second.size } );
        }
    }
    template<class A, uint64_t flags>
    Blk AllocatorWithStats<A, flags>::Alloc( std::size_t s, const std::source_location& location )
    {
        Blk b = m_allocator.Alloc( s );
        if ( b.ptr )
        {
            AllocationInfo info;
            info.location  = location;
            info.timestamp = std::time( nullptr );
            info.size      = s;

            m_allocationInfoMap[ b.ptr ] = info;
        }
        return b;
    }
    template<class A, uint64_t flags>
    void AllocatorWithStats<A, flags>::Free( Blk b )
    {
        auto it = m_allocationInfoMap.find( b.ptr );
        if ( it != m_allocationInfoMap.end() )
        {
            m_allocator.Free( b );
            m_allocationInfoMap.erase( it );
        }
    }
    template<class A, uint64_t flags>
    bool AllocatorWithStats<A, flags>::Owns( Blk b )
    {
        return m_allocator.Owns( b );
    }
}// namespace FawnMemAlloc