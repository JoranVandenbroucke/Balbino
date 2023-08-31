//
// Created by joran on 16/08/2023.
//

#pragma once
namespace FawnMemAlloc
{
#if defined __clang__ || defined __GNUC__
#define PRETTY_FUNCTION __PRETTY_FUNCTION__
#define PRETTY_FUNCTION_PREFIX '='
#define PRETTY_FUNCTION_SUFFIX ']'
#elif defined _MSC_VER
#define PRETTY_FUNCTION __FUNCSIG__
#define PRETTY_FUNCTION_PREFIX '<'
#define PRETTY_FUNCTION_SUFFIX '>'
#endif

    struct Blk {
        void* ptr;
        size_t size;
    };
}// namespace FawnMemAlloc