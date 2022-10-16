#pragma once
#include "Base.h"

#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
    class CImageView;
    
    class CRenderPass;
    
    class CFrameBuffer final : public CDeviceObject
    {
    public:
        explicit CFrameBuffer( const CDevice* device );
        CFrameBuffer( CFrameBuffer&& ) = default;
        CFrameBuffer& operator=( CFrameBuffer&& ) = default;
        ~CFrameBuffer() override;
        
        //the framebuffer assumes that rendering is forward
        void Initialize( BalVulkan::CRenderPass* pRenderPass, uint32_t width, uint32_t height, const std::vector<CImageView*>& renderTargets, CImageView* pDepth );
        
        VkFramebuffer GetFrameBuffer( uint32_t idx ) const;
        static CFrameBuffer* CreateNew( const CDevice* pDevice );
    private:
        std::vector<VkFramebuffer> m_frameBuffer;
        
    };
}
