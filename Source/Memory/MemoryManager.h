//
// Created by joran on 15/08/2023.
//
#ifdef USE_MEMORY_MANAGER
#pragma once
#include "MemoryManagerCore.h"

#include "Allocators/AffixAllocator.h"
#include "Allocators/AllocatorWithStats.h"
#include "Allocators/DoubleBufferAllocator.h"
#include "Allocators/FallbackAllocator.h"
#include "Allocators/FreelistAllocator.h"
#include "Allocators/Mallocator.h"
#include "Allocators/PoolAllocator.h"
#include "Allocators/StackAllocator.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <map>
#include <memory>
#include <string_view>
#include <utility>

//// Organize by constant-size blocks
//// Tremendously simpler than malloc's heap
//// Faster, too
//// Layered on top of any memory hunk
//// 1 bit/block solve overhead (!)
//// Multithreading tenuous (needs full interlocking for > 1 block)
//template<class A, std::size_t blockSize>
//class BitmappedBlock;
//
//// Keep a list of allocators, grow lazily
//// Coarse granularity
//// Linear search upon allocation
//// May return memory back
//template<class Creator>
//class CascadingAllocator;
//// auto a = cascadingAllocator({ return Heap<...>();});
//
//// Size <= threshold goes to SmallAllocator
//// All else goes to LargeAllocator
//template<std::size_t  threshold, class SmallAllocator, class LargeAllocator>
//struct Segragator;
//
////typedef Segragator<4096,Segragator<128, FreeListAllocator<Mallocator, 0, 128>, MediumAllocator> Mallocator> Allocator;
//
//template<class Allocator, std::size_t min, std::size_t max, std::size_t step>
//struct Bucketizer;

namespace FawnMemAlloc
{
#ifdef _DEBUG
    using Allocator = AllocatorWithStats<FallbackAllocator<FreelistAllocator<StackAllocator<16384>, 9, 64, 1, 1024>, Mallocator>, 0>;
#else
    using Allocator = FallbackAllocator<FreelistAllocator<StackAllocator<16384>, 9, 64, 1, 1024>, Mallocator>;
#endif
    namespace
    {
        template<typename T>
        uint32_t Hash()
        {
            std::string_view functionName { PRETTY_FUNCTION };
            uint64_t first         = functionName.find_first_not_of( ' ', functionName.find_first_of( PRETTY_FUNCTION_PREFIX ) + 1 );
            std::string_view value = functionName.substr( first, functionName.find_last_of( PRETTY_FUNCTION_SUFFIX ) - first );
            return std::hash<std::string_view> {}( value );
        }
    }// namespace

    class MemoryManager
    {
    public:
        MemoryManager()  = default;
        ~MemoryManager() = default;

        template<typename T, typename... Args>
        T* Allocate( Args&&... args );
        template<typename T>
        void Free( T* ptr );

#ifdef _DEBUG
        void* Allocate( std::size_t n, std::source_location location = std::source_location::current() );
#else
        void* Allocate( std::size_t n );
#endif
        void Free( void* ptr, std::size_t size = 0 );

    private:
        std::map<uint32_t, std::shared_ptr<void>> m_assetMap {};
        Allocator m_allocator {};

        template<typename T>
        PoolAllocator<T>& GetAllocator( uint32_t id = Hash<T>() );
    };

    template<typename T, typename... Args>
    T* MemoryManager::Allocate( Args&&... args )
    {
        return GetAllocator<T>().Alloc( std::forward<Args>( args )... );
    }
    template<typename T>
    void MemoryManager::Free( T* ptr )
    {
        return GetAllocator<T>().Free( ptr );
    }
    template<typename T>
    PoolAllocator<T>& MemoryManager::GetAllocator( uint32_t id )
    {
        static_assert( std::is_same_v<T, std::decay_t<T>>, "Non Decay types are not allowed!" );

        auto it = m_assetMap.find( id );
        if ( it == m_assetMap.end() )
        {
            auto pool        = std::make_shared<PoolAllocator<T>>( 4 );
            m_assetMap[ id ] = pool;
            return *std::static_pointer_cast<PoolAllocator<T>>( pool );
        }

        return *std::static_pointer_cast<PoolAllocator<T>>( it->second );
    }
    MemoryManager g_memoryManager {};

}// namespace FawnMemAlloc

#ifdef _DEBUG
[[nodiscard]] void* operator new( std::size_t count, std::source_location location = std::source_location::current() );
[[nodiscard]] void* operator new[]( std::size_t count, std::source_location location = std::source_location::current() );
#else
[[nodiscard]] void* operator new( std::size_t count );
[[nodiscard]] void* operator new[]( std::size_t count );
#endif

void operator delete( void* ptr );
void operator delete[]( void* ptr );

void operator delete( void* ptr, std::size_t size );
void operator delete[]( void* ptr, std::size_t size );
#endif