#pragma once
#include <vulkan/vulkan.hpp>
#include "Base.h"


namespace FawnVision
{
    class CImageView;
    class CRenderPass;
    class CFrameBuffer final : public CDeviceObject
    {
    public:
        explicit CFrameBuffer( const CDevice* pDevice );
        ~CFrameBuffer() override;
        
        //the framebuffer assumes that rendering is forward
        void Initialize( CRenderPass* pRenderPass, uint32_t width, uint32_t height, const std::vector<CImageView*>& renderTargets, CImageView* pDepth );
        
        VkFramebuffer GetFrameBuffer( uint32_t idx ) const;
    private:
        std::vector<VkFramebuffer> m_frameBuffer;
        
    };
}
