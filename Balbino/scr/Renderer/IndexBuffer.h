#pragma once

namespace BalVulkan
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
        
        void Initialize( const std::vector<uint32_t>& indices, const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue );
        void Cleanup();
        void Bind() const;
    private:
        BalVulkan::CBuffer* m_pIndexBuffer;
    };
}
