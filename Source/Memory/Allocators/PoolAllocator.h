//
// Created by joran on 15/08/2023.
//

#pragma once
#include "../MemoryManagerCore.h"
#include <cstdint>
#include <numeric>
#include <vector>

namespace FawnMemAlloc
{
    template<typename T>
    class PoolAllocator
    {
    public:
        explicit PoolAllocator( size_t firstBlockCapacity );
        ~PoolAllocator();

        template<typename... Args>
        T* Alloc( Args&&... args );
        void Free( T* ptr );

    private:
        struct Item {
            uint32_t nextIndex;
            alignas( T ) char value[ sizeof( T ) ];
        };
        struct Block {
            Item* pItems;
            uint32_t capacity;
            uint32_t firstFreeIndex;
        };

        std::vector<Block> m_blocks;
        const uint32_t m_firstBlockCapacity;

        Block& CreateNewBlock()
        {
            const uint32_t newBlockCapacity = m_blocks.empty() ? m_firstBlockCapacity : m_blocks.back().Capacity * 3 / 2;
            const Block& newBlock { m_blocks.emplace_back( newBlockCapacity ) };

            // Setup singly-linked list of all free items in this block.
            for ( uint32_t i = 0; i < newBlockCapacity - 1; ++i )
            {
                newBlock.pItems[ i ].NextFreeIndex = i + 1;
            }
            newBlock.pItems[ newBlockCapacity - 1 ].NextFreeIndex = UINT32_MAX;
            return newBlock;
        }
    };
    template<typename T>
    PoolAllocator<T>::PoolAllocator( size_t firstBlockCapacity )
        : m_firstBlockCapacity( firstBlockCapacity )
    {
    }
    template<typename T>
    PoolAllocator<T>::~PoolAllocator()
    {
        for ( size_t i {}; i < m_blocks.size(); ++i )
        {
            delete[] m_blocks[ i ].pItems->value;
        }
        m_blocks.clear();
    }
    template<typename T>
    template<typename... Args>
    T* PoolAllocator<T>::Alloc( Args&&... args )
    {
        for ( size_t i { m_blocks.size() }; --i; )
        {
            Block& block = m_blocks;
            if ( block.firstFreeIndex != UINT32_MAX )
            {
                Item* pItem          = &block.pItems[ block.firstFreeIndex ];
                block.firstFreeIndex = pItem->nextIndex;
                T* tmp { (T*)&pItem->value };
                new ( tmp ) T( std::forward<Args>( args )... );
                return tmp;
            }
        }

        Block& block         = CreateNewBlock();
        Item* pItem          = block.pItems;
        block.firstFreeIndex = pItem->nextIndex;
        T* tmp { (T*)&pItem->value };
        new ( tmp ) T( std::forward<Args>( args )... );
        return tmp;
    }
    template<typename T>
    void PoolAllocator<T>::Free( T* ptr )
    {
        for ( size_t i { m_blocks.size() }; --i; )
        {
            Block& block { m_blocks[ i ] };
            Item* pItem;
            std::memcpy( &pItem, &ptr, sizeof( pItem ) );

            if ( ( pItem >= block.pItems ) && ( pItem < block.pItems + block.capacity ) )
            {
                ptr->~T();
                const uint32_t idx { static_cast<uint32_t>( pItem - block.pItems ) };
                pItem->nextIndex     = block.firstFreeIndex;
                block.firstFreeIndex = idx;
            }
        }
    }
}