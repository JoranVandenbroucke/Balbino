#pragma once
#include <vector>

#include "MeshMetadata.h"
#include "UUID.h"

namespace BalVulkan
{
    class CQueue;
    
    class CCommandPool;
    
    class CDevice;
}

namespace Balbino
{
    struct SMeshMetadata;
    
    struct IMesh
    {
    public:
        IMesh() = default;
        virtual ~IMesh() = default;
        IMesh( const IMesh& ) = delete;
        IMesh( IMesh&& ) = delete;
        IMesh& operator=( const IMesh& ) = delete;
        IMesh& operator=( IMesh&& ) = delete;
        
        virtual void Initialize( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue ) = 0;
        virtual void Cleanup() = 0;
        virtual void Bind() const = 0;
        
        [[nodiscard]] virtual CUuid GetUuid() const = 0;
        [[nodiscard]] virtual const std::vector<Balbino::SMeshMetadata>& GetMetaData() const = 0;
        [[nodiscard]] virtual const uint32_t GetMaterialCount() const = 0;
    };
}
