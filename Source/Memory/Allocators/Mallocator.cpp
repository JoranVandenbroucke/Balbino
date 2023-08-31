//
// Created by joran on 18/08/2023.
//
#include "Mallocator.h"
#include <cstdlib>

namespace FawnMemAlloc
{
    Blk Mallocator::Alloc( size_t n )
    {
        return { malloc( n ), n };
    }
    void Mallocator::Free( Blk b )
    {
        free( b.ptr );
    }
    bool Mallocator::Owns( Blk b )
    {
        (void)b;
        return true;
    }
}