#pragma once
#include "FawnVisionCore.h"
#include <iostream>

namespace FawnVision
{
    class CIndexBuffer final {
    public:
        constexpr CIndexBuffer()=default;
        constexpr ~CIndexBuffer()
        {
            if ( m_pIndexBuffer != BALBINO_NULL )
            {
                std::cerr << "index buffer was not destroyed" << std::endl;
            }
        }
        constexpr CIndexBuffer( const CIndexBuffer& ) = delete;
        constexpr CIndexBuffer( CIndexBuffer&& ) = delete;
        constexpr CIndexBuffer& operator=( const CIndexBuffer& ) = delete;
        constexpr CIndexBuffer& operator=( CIndexBuffer&& ) = delete;
        
        void Initialize( const std::vector<uint32_t>& indices, FawnVision::Device device, FawnVision::CommandPool commandPool, FawnVision::Queue queue );
        void Cleanup();
        void Bind() const;
    private:
        FawnVision::Buffer m_pIndexBuffer{ BALBINO_NULL };
    };
}
