#pragma once
#include <Camera.h>
#include <CameraComponent.h>
#include <Core.h>
#include <LightComponent.h>
#include <MeshRendererComponent.h>
#include <TransformComponent.h>
#include <UUID.h>

#include "FawnVisionCore.h"
#include "UBOStructs.h"

#include <unordered_map>

struct SDL_Window;
namespace FawnVision
{
    namespace
    {
#if defined __clang__ || defined __GNUC__
#define PRETTY_FUNCTION __PRETTY_FUNCTION__
#define PRETTY_FUNCTION_PREFIX '='
#define PRETTY_FUNCTION_SUFFIX ']'
#elif defined _MSC_VER
#define PRETTY_FUNCTION __FUNCSIG__
#define PRETTY_FUNCTION_PREFIX '<'
#define PRETTY_FUNCTION_SUFFIX '>'
#endif
        template<typename T>
        size_t Hash()
        {
            std::string_view functionName { PRETTY_FUNCTION };
            uint64_t first         = functionName.find_first_not_of( ' ', functionName.find_first_of( PRETTY_FUNCTION_PREFIX ) + 1 );
            std::string_view value = functionName.substr( first, functionName.find_last_of( PRETTY_FUNCTION_SUFFIX ) - first );
            return std::hash<std::string_view> {}( value );
        }
    }// namespace

    class CTexture;
    class CMaterial;
    class CMesh;

    class CRenderer
    {
    public:
        CRenderer()  = default;
        ~CRenderer() = default;

        constexpr CRenderer( const CRenderer& )            = delete;
        constexpr CRenderer( CRenderer&& )                 = delete;
        constexpr CRenderer& operator=( const CRenderer& ) = delete;
        constexpr CRenderer& operator=( CRenderer&& )      = delete;

#ifdef BALBINO_EDITOR
        void SetupEditor();
        void SetupEditorFont();
        void CleanupEditor();
#endif// BALBINO_EDITOR

        void Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount );
        void Cleanup();
        bool StartDraw() noexcept;
        bool EndDraw() noexcept;

        [[nodiscard]] FawnVision::ImageResource CreateImage( const FawnVision::SImageCreateInfo& createInfo ) const;
        [[nodiscard]] FawnVision::ImageView CreateImageView( ImageResource pImage, const FawnVision::SImageViewCreateInfo& createInfo ) const;
        [[nodiscard]] FawnVision::Sampler CreateImageSampler( const FawnVision::SSamplerCreateInfo& createInfo ) const;
        [[nodiscard]] FawnVision::Buffer CreateBuffer( uint64_t size, buffer_usage_flag bufferUsage, memory_property_flag memoryProperty );
        [[nodiscard]] FawnVision::Shader CreateShader( const FawnVision::SShaderCreateInfo& createInfo );
        [[nodiscard]] FawnVision::ShaderPipeline CreateShaderPipeline( FawnVision::SShaderPassCreateInfo createInfo, const std::vector<FawnVision::Shader>& shaders );

        [[nodiscard]] inline FawnVision::ImageResource CreateImage( FawnVision::image_view_type type, FawnVision::image_format format, uint32_t width, uint32_t height, uint32_t depth, uint8_t mips, uint8_t layers, uint32_t sampleLevel,
                                                                    image_usage_flag usage, FawnVision::image_layout layout = FawnVision::image_layout::undefined ) const
        {
            const FawnVision::SImageCreateInfo createInfo {
                .arrayLayers   = layers,
                .depth         = depth,
                .height        = height,
                .initialLayout = (uint32_t)layout,
                .mipLevels     = mips,
                .samples       = sampleLevel,
                .usage         = usage,
                .width         = width,
                .format        = (uint8_t)format,
                .imageType     = (uint8_t)type,
            };
            return CreateImage( createInfo );
        }
        [[nodiscard]] inline ImageView CreateImageView( ImageResource pImage, image_view_type type, uint32_t firstMip, uint32_t numMips, uint32_t firstLayer, uint32_t numLayers ) const
        {
            const FawnVision::SImageViewCreateInfo createInfo {
                .componentMapping = ( (uint32_t)component_swizzle::swizzle_r ) | ( (uint32_t)component_swizzle::swizzle_g << 8 ) | ( (uint32_t)component_swizzle::swizzle_b << 16 ) | ( (uint32_t)component_swizzle::swizzle_a << 24 ),
                .firstLayer       = firstLayer,
                .firstMip         = firstMip,
                .layerCount       = numLayers,
                .mipCount         = numMips,
                .imageViewType    = (uint8_t)type,
            };
            return CreateImageView( pImage, createInfo );
        }
        [[nodiscard]] inline Sampler CreateImageSampler( uint32_t filterMode, uint32_t mipmapMode, sampler_address_mode samplerAddressModeU, sampler_address_mode samplerAddressModeV, sampler_address_mode samplerAddressModeW, uint32_t anisotropy,
                                                         uint32_t mipLevels )
        {
            const FawnVision::SSamplerCreateInfo createInfo {
                .maxAnisotropy           = (float)anisotropy,
                .maxLod                  = (float)mipLevels,
                .minLod                  = 0.0f,
                .mipLodBias              = 0.0f,
                .compareEnable           = 0u,
                .unNormalizedCoordinates = 0u,
                .addressModeU            = (uint8_t)samplerAddressModeU,
                .addressModeV            = (uint8_t)samplerAddressModeV,
                .addressModeW            = (uint8_t)samplerAddressModeW,
                .minFilter               = (uint8_t)mipmapMode,
                .mipMapMode              = 1u,
                .borderColor             = 3u,
                .compareOperator         = 7u,
                .magFilter               = (uint8_t)filterMode,
            };
            return CreateImageSampler( createInfo );
        }

        [[nodiscard]] FawnVision::CTexture* CreateTexture( const CUuid& id, FawnVision::STextureCreateInfo createInfo, bool generateMips, uint32_t type = Hash<CTexture>() );
        [[nodiscard]] FawnVision::CMaterial* CreateMaterial( const CUuid& id, FawnVision::ShaderPipeline pipeline, const FawnVision::SMaterialCreateInfo& createInfo, uint32_t type = Hash<CMaterial>() );
        [[nodiscard]] FawnVision::CMesh* CreateMesh( const CUuid& id, const FawnVision::SMeshCreateInfo& createInfo, uint32_t type = Hash<CTexture>() );

        void DrawMesh( uint32_t indexCount, uint32_t firstIndex, uint32_t instanceCount, uint32_t firstInstance );
        void UpdateModelBuffer( SModelObject object, size_t size );
        void UpdateLightObject( SLightObject object, size_t size );

        [[nodiscard]] inline const FawnVision::Buffer GetModelBuffer() const noexcept
        {
            return m_pModelBuffer;
        }
        constexpr FawnVision::Buffer GetModelBuffer() noexcept
        {
            return m_pModelBuffer;
        }

        [[nodiscard]] inline const FawnVision::Buffer GetShadingBuffer() const noexcept
        {
            return m_pLightBuffer;
        }
        constexpr FawnVision::Buffer GetShadingBuffer() noexcept
        {
            return m_pLightBuffer;
        }

        void PrepareDraw( const std::pair<BalbinoComponent::CTransformComponent, BalbinoComponent::CCameraComponent>& cameras, const std::vector<std::pair<BalbinoComponent::CTransformComponent, BalbinoComponent::CLightComponent>>& lights,
                          const std::vector<std::tuple<BalbinoComponent::CTransformComponent, BalbinoComponent::CMeshRenderComponent, std::vector<SMeshMetadata>>>& meshes );

        void ExecuteRenderPass( int index );

        template<typename T>
        T* GetAsset( const CUuid& id, uint32_t type = Hash<T>() )
        {
            if ( m_assetMap.find( type ) != m_assetMap.cend() && m_assetMap.at( type ).find( id ) != m_assetMap.at( type ).end() )
                return (T*)( m_assetMap.at( type ).at( id ) );
            return nullptr;
        }
        template<typename T>
        const T* GetAsset( const CUuid& id, uint32_t type = Hash<T>() ) const
        {
            if ( m_assetMap.find( type ) != m_assetMap.cend() && m_assetMap.at( type ).find( id ) != m_assetMap.at( type ).end() )
                return (T*)( m_assetMap.at( type ).at( id ) );
            return nullptr;
        }
        void DrawEditor();

    private:
        FawnVision::CommandPool m_commandPool { BALBINO_NULL };
        FawnVision::Device m_device { BALBINO_NULL };
        FawnVision::FrameBuffer m_frameBuffer { BALBINO_NULL };
        FawnVision::ImageResource m_depthImage { BALBINO_NULL };
        FawnVision::ImageView m_depthImageView { BALBINO_NULL };
        FawnVision::Instance m_instance { BALBINO_NULL };
        FawnVision::Queue m_queue { BALBINO_NULL };
        FawnVision::RenderPass m_renderPass { BALBINO_NULL };
        FawnVision::Semaphore m_signalingSemaphore { BALBINO_NULL };
        FawnVision::Semaphore m_waitingSemaphore { BALBINO_NULL };
        FawnVision::Swapchain m_swapchain { BALBINO_NULL };

        std::vector<FawnVision::Fence> m_fences;
        std::vector<FawnVision::Fence> m_inFlightFences;
        std::vector<FawnVision::ImageResource> m_swapchainResources;
        std::vector<FawnVision::ImageView> m_swapchainViews;

        std::vector<SIndirectBatch> m_allDrawableObjects {};
        std::vector<std::vector<FawnVision::SInstanceBatch>> m_instanceData {};

        std::unordered_map<size_t, std::unordered_map<CUuid, void*>> m_assetMap;

        SLightObject m_lightObject {};
        BalbinoScene::CCamera m_camera {};

#ifdef BALBINO_EDITOR
        VkDescriptorPool m_editorPool;
#endif

        uint32_t m_imageIndex {};
        int32_t m_width {};
        int32_t m_height {};

        Buffer m_pModelBuffer { BALBINO_NULL };
        Buffer m_pLightBuffer { BALBINO_NULL };
        Buffer m_pInstanceBuffer { BALBINO_NULL };

        SDL_Window* m_pWindow { BALBINO_NULL };

        void RecreateSwapChain() noexcept;
        void CreateModelBuffer() noexcept;
        void CreateLightBuffer() noexcept;
        void CreateInstanceBuffer() noexcept;

        void UpdateInstanceObject( const std::vector<FawnVision::SInstanceBatch>& instanceData );

        void ProcessCamera( const std::pair<BalbinoComponent::CTransformComponent, BalbinoComponent::CCameraComponent>& cameras );
        void ProcessLights( const std::vector<std::pair<BalbinoComponent::CTransformComponent, BalbinoComponent::CLightComponent>>& lights );
        void ProcessMeshes( const std::vector<std::tuple<BalbinoComponent::CTransformComponent, BalbinoComponent::CMeshRenderComponent, std::vector<SMeshMetadata>>>& meshes );
    };
}// namespace FawnVision
