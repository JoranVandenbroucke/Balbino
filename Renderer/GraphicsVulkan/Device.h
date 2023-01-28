#pragma once
#include "Base.h"

#include "Buffer.h"
#include "ImageView.h"
#include "Sampler.h"

namespace BalVulkan
{
    class CInstance;
    
    struct SPhysicalDeviceInfo;
    
    class CDeviceHolder
    {
    public:
        CDeviceHolder( VkDevice device, VkAllocationCallbacks* hostAllocator );
        CDeviceHolder( const CDeviceHolder& ) = delete;
        CDeviceHolder( CDeviceHolder&& ) = delete;
        CDeviceHolder& operator=( const CDeviceHolder& ) = delete;
        CDeviceHolder& operator=( CDeviceHolder&& ) = delete;
        virtual ~CDeviceHolder();
    
    protected:
        VkAllocationCallbacks* m_pCallbacks;
        VkDevice m_device;
    };
    
    class CDevice final : public CDeviceHolder, public CRefCounted
    {
    public:
        CDevice( const SPhysicalDeviceInfo* pDeviceInfo, VkAllocationCallbacks* pCallbacks, VkDevice device );
        ~CDevice() override;
        
        void WaitIdle() const;
        
        const VkDevice& GetVkDevice() const;
        const SPhysicalDeviceInfo* GetPhysicalDeviceInfo() const;
        
        
        static CDevice* Create( const SPhysicalDeviceInfo* pDeviceInfo, VkAllocationCallbacks* pCallbacks, const std::vector<const char*>& layersToEnable, const std::vector<const char*>& extensionsToEnable );
    private:
        const SPhysicalDeviceInfo* m_pDeviceInfo;
        
        struct SRenderPass
        {
            const VkDevice     & self;
            const VkRenderPass & renderPass;
            const VkFramebuffer& frameBuffer;
            SRenderPass( SRenderPass&& ) noexcept;
            SRenderPass( const VkDevice& s, const VkRenderPass& r, const VkFramebuffer& f );
            
            ~SRenderPass();
        };
        
        struct SPipeline
        {
            const VkDevice  & self;
            const VkPipeline& pipeline;
            SPipeline( SPipeline&& ) noexcept;
            SPipeline( const VkDevice& s, const VkPipeline& p );
            
            ~SPipeline();
        };
        
        static constexpr uint32_t s_deferredSize{ 2 };
        //std::vector<CBuffer>		m_deferredBufferViews[s_deferredSize];
        //std::vector<CImageView>		m_deferredImageViews[s_deferredSize];
        //std::vector<CSampler>		m_deferredSamplers[s_deferredSize];
        //std::vector<SRenderPass>	m_deferredRenderPasses[s_deferredSize];
        //std::vector<SPipeline>		m_deferredPipelines[s_deferredSize];
    };
}
