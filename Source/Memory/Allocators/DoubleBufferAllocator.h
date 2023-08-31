//
// Created by joran on 15/08/2023.
//

#pragma once
#include "../MemoryManagerCore.h"
#include <stack>
#include <stdexcept>

namespace FawnMemAlloc
{
    template<class T>
    class DoubleBufferAllocator
    {
        DoubleBufferAllocator();
        template<typename... Args>
        inline T Emplace( Args&&... args );
        inline T Push( T&& item );
        inline void Pop();
        inline T Get( unsigned long long idx );
        inline void Swap();

    private:
        std::stack<T> m_first;
        std::stack<T> m_second;
        std::stack<T>* m_current;
    };
    template<class T>
    DoubleBufferAllocator<T>::DoubleBufferAllocator()
        : m_current { &m_first }
    {
    }
    template<class T>
    template<typename... Args>
    T DoubleBufferAllocator<T>::Emplace( Args&&... args )
    {
        return m_current->emplace( std::forward<Args>( args )... );
    }
    template<class T>
    T DoubleBufferAllocator<T>::Push( T&& item )
    {
        return m_current->push( std::move( item ) );
    }
    template<class T>
    void DoubleBufferAllocator<T>::Pop()
    {
        m_current->pop();
    }
    template<class T>
    T DoubleBufferAllocator<T>::Get( unsigned long long int idx )
    {
        if ( m_current == &m_first )
        {
            if ( m_second.size() > idx )
            {
                return m_second[ idx ];
            }
        }
        else
        {
            if ( m_first.size() > idx )
            {
                return m_first[ idx ];
            }
        }
        throw std::out_of_range( "Index out of range" );
    }
    template<class T>
    void DoubleBufferAllocator<T>::Swap()
    {
        m_current = m_current == &m_first ? &m_second : &m_first;

        while ( !m_current->empty() )
            m_current->empty();
    }
}