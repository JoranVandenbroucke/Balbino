//
// Created by joran on 16/08/2023.
//

#pragma once
#include "../MemoryManagerCore.h"
#include <type_traits>

namespace FawnMemAlloc
{
    // Add optimal prefix and suffix
    // Construct/destroy appropriately
    // Uses: debug, stats, info, ...
    template<class A, class Prefix, class Suffix = void>
    class AffixAllocator
    {
    public:
        Blk Alloc( size_t n );
        void Free( Blk b );
        bool Owns( Blk b );

    private:
        A m_allocator;

        size_t ActualAllocationSize( size_t n );
        size_t roundUpToMultipleOf( size_t a, size_t b );
    };
    template<class A, class Prefix, class Suffix>
    size_t AffixAllocator<A, Prefix, Suffix>::roundUpToMultipleOf( size_t a, size_t b )
    {
        if ( b == 0 )
            return a;

        int remainder = a % b;
        if ( remainder == 0 )
            return a;

        return a + b - remainder;
    }
    template<class A, class Prefix, class Suffix>
    size_t AffixAllocator<A, Prefix, Suffix>::ActualAllocationSize( size_t n )
    {
        return ( n + sizeof( Prefix ), std::alignment_of<Suffix>::value );
    }
    template<class A, class Prefix, class Suffix>
    Blk AffixAllocator<A, Prefix, Suffix>::Alloc( size_t n )
    {
        if ( !n )
            return { nullptr, 0 };

        size_t prefixSize = sizeof( Prefix );
        size_t suffixSize = sizeof( Suffix );
        size_t dataSize   = n - prefixSize - suffixSize;

        Blk rawData = m_allocator.Allocate( n );

        Prefix* prefix = new ( rawData.ptr ) Prefix {};
        void* data     = static_cast<char*>( rawData.ptr ) + prefixSize;
        Suffix* suffix = new ( static_cast<char*>( data ) + dataSize ) Suffix {};

        return { data, dataSize };
    }
    template<class A, class Prefix, class Suffix>
    void AffixAllocator<A, Prefix, Suffix>::Free( Blk b )
    {
        char* originalStart = static_cast<char*>( b.ptr ) - sizeof( Prefix );

        Prefix* prefix = reinterpret_cast<Prefix*>( originalStart );
        Suffix* suffix = reinterpret_cast<Suffix*>( static_cast<char*>( b.ptr ) + b.size );

        prefix->~Prefix();
        suffix->~Suffix();

        m_allocator.Free( { originalStart, b.size + sizeof( Prefix ) + sizeof( Suffix ) } );
    }
    template<class A, class Prefix, class Suffix>
    bool AffixAllocator<A, Prefix, Suffix>::Owns( Blk b )
    {
        char* originalStart = static_cast<char*>( b.ptr ) - sizeof( Prefix );
        return m_allocator.Owns( { originalStart, b.size + sizeof( Prefix ) + sizeof( Suffix ) } );
    }
}