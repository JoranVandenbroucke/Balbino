#pragma once
#include <vector>

namespace FawnVision
{
    struct SVertex;
    
    class CDevice;
    
    class CCommandPool;
    
    class CQueue;
    
    class CBuffer;
}

namespace Balbino
{
    class CVertexBuffer
    {
    public:
        CVertexBuffer();
        ~CVertexBuffer();
        CVertexBuffer( const CVertexBuffer& ) = delete;
        CVertexBuffer( CVertexBuffer&& ) = delete;
        CVertexBuffer& operator=( const CVertexBuffer& ) = delete;
        CVertexBuffer& operator=( CVertexBuffer&& ) = delete;
        
        void Initialize( const std::vector<FawnVision::SVertex>& vertices, const FawnVision::CDevice* pDevice, const FawnVision::CCommandPool* pCommandPool, const FawnVision::CQueue* pQueue );
        void Cleanup();
        void Bind() const;
    private:
        FawnVision::CBuffer* m_vertexBuffer;
    };
}
