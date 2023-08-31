#pragma once
#include "FawnVisionCore.h"
#include <iostream>

namespace FawnVision
{
    class CVertexBuffer final
    {
    public:
        constexpr CVertexBuffer()=default;
        constexpr ~CVertexBuffer()
        {
            if ( m_vertexBuffer != BALBINO_NULL )
            {
                std::cerr << "index buffer was not destroyed" << std::endl;
            }
        }
        constexpr CVertexBuffer( const CVertexBuffer& ) = delete;
        constexpr CVertexBuffer( CVertexBuffer&& ) = delete;
        constexpr CVertexBuffer& operator=( const CVertexBuffer& ) = delete;
        constexpr CVertexBuffer& operator=( CVertexBuffer&& ) = delete;
        
        void Initialize( const std::vector<FawnVision::SVertex>& vertices, FawnVision::Device device, FawnVision::CommandPool commandPool, FawnVision::Queue queue );
        void Cleanup();
        void Bind() const;
    private:
        FawnVision::Buffer m_vertexBuffer{ BALBINO_NULL };
    };
}
