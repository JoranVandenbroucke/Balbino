//
// Created by joran on 16/08/2023.
//

#pragma once
#include "../MemoryManagerCore.h"

namespace FawnMemAlloc
{
    class Mallocator
    {
    public:
        Blk Alloc( std::size_t n );
        void Free( Blk b );
        bool Owns( Blk b );
    };
}// namespace FawnMemAlloc