#pragma once

namespace FawnVision
{
    class CDevice;
    
    class CCommandPool;
    
    class CQueue;
    
    class CBuffer;
}

namespace Balbino
{
    class CIndexBuffer
    {
    public:
        CIndexBuffer();
        ~CIndexBuffer();
        CIndexBuffer( const CIndexBuffer& ) = delete;
        CIndexBuffer( CIndexBuffer&& ) = delete;
        CIndexBuffer& operator=( const CIndexBuffer& ) = delete;
        CIndexBuffer& operator=( CIndexBuffer&& ) = delete;
        
        void Initialize( const std::vector<uint32_t>& indices, const FawnVision::CDevice* pDevice, const FawnVision::CCommandPool* pCommandPool, const FawnVision::CQueue* pQueue );
        void Cleanup();
        void Bind() const;
    private:
        FawnVision::CBuffer* m_pIndexBuffer;
    };
}
