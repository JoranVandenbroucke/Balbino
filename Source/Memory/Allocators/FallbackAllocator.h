//
// Created by joran on 16/08/2023.
//

#pragma once
#include "../MemoryManagerCore.h"

namespace FawnMemAlloc
{
    template<class Primary, class Fallback>
    class FallbackAllocator : private Primary, private Fallback
    {
    public:
        Blk Alloc( size_t n );
        void Free( Blk b );
        bool Owns( Blk b );
    };
    template<class Primary, class Fallback>
    Blk FallbackAllocator<Primary, Fallback>::Alloc( size_t n )
    {
        Blk r = Primary::Alloc( n );
        if ( r.ptr )
            r = Fallback::Alloc( n );
        return r;
    }
    template<class Primary, class Fallback>
    void FallbackAllocator<Primary, Fallback>::Free( Blk b )
    {
        if ( Primary::Owns( b ) )
            Primary::Free( b );
        else if ( Fallback::Owns( b ) )
            Fallback::Free( b );
    }
    template<class Primary, class Fallback>
    bool FallbackAllocator<Primary, Fallback>::Owns( Blk b )
    {
        return Primary::Owns( b ) || Fallback::Owns( b );
    }
}