#include "Renderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include <DeerVulkan.h>
#include <SDL3/SDL_vulkan.h>

constexpr uint32_t MAX_INSTANCE_COUNT { 1024 };

#ifdef BALBINO_EDITOR
#define IMGUI_DEFINE_MATH_OPERATORS
#include <backends/imgui_impl_vulkan.h>
void FawnVision::CRenderer::SetupEditor()
{
    constexpr VkDescriptorPoolSize poolSizes[] { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                                                 { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };
    const VkDescriptorPoolCreateInfo poolInfo {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .maxSets       = 1000 * IM_ARRAYSIZE( poolSizes ),
        .poolSizeCount = static_cast<uint32_t>( IM_ARRAYSIZE( poolSizes ) ),
        .pPoolSizes    = poolSizes,
    };

    vkCreateDescriptorPool( m_device->GetVkDevice(), &poolInfo, VK_NULL_HANDLE, &m_editorPool );

    ImGui_ImplVulkan_InitInfo info {
        .Instance        = VK_NULL_HANDLE,//todo: maybe?
        .PhysicalDevice  = m_device->GetPhysicalDeviceInfo().device,
        .Device          = m_device->GetVkDevice(),
        .QueueFamily     = m_queue->GetQueFamily(),
        .Queue           = m_queue->GetHandle(),
        .PipelineCache   = VK_NULL_HANDLE,
        .DescriptorPool  = m_editorPool,
        .Subpass         = 0,
        .MinImageCount   = m_swapchain->GetMinImage(),
        .ImageCount      = m_swapchain->GetImageCount(),
        .Allocator       = VK_NULL_HANDLE,
        .CheckVkResultFn = VK_NULL_HANDLE,
    };
    ImGui_ImplVulkan_Init( &info, m_renderPass->GetHandle() );
}
void FawnVision::CRenderer::SetupEditorFont()
{
    VkCommandPool commandPool     = m_commandPool->GetHandle();
    VkCommandBuffer commandBuffer = m_commandPool->GetCommandBuffer();

    vkResetCommandPool( m_device->GetVkDevice(), commandPool, 0 );
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer( commandBuffer, &beginInfo );

    ImGui_ImplVulkan_CreateFontsTexture( commandBuffer );

    VkSubmitInfo endInfo {};
    endInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    endInfo.commandBufferCount = 1;
    endInfo.pCommandBuffers    = &commandBuffer;
    vkEndCommandBuffer( commandBuffer );
    vkQueueSubmit( m_queue->GetHandle(), 1, &endInfo, VK_NULL_HANDLE );

    m_device->WaitIdle();
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}
void FawnVision::CRenderer::DrawEditor()
{
    ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData(), m_commandPool->GetCommandBuffer() );
}
void FawnVision::CRenderer::CleanupEditor()
{
    vkDestroyDescriptorPool( m_device->GetVkDevice(), m_editorPool, VK_NULL_HANDLE );
}
#endif

void FawnVision::CRenderer::Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount )
{
    m_pWindow = pWindow;
    VkSurfaceKHR surface;
    uint32_t imageResourceSize;
    std::vector<FawnVision::image_format> formats;

    const FawnVision::SImageViewCreateInfo depthViewInfo {
        .componentMapping = ( (uint32_t)component_swizzle::swizzle_r ) | ( (uint32_t)component_swizzle::swizzle_g << 8 ) | ( (uint32_t)component_swizzle::swizzle_b << 16 ) | ( (uint32_t)component_swizzle::swizzle_a << 24 ),
        .imageViewType    = (uint8_t)FawnVision::image_view_type::view_2d,
    };

    m_instance = FawnVision::CreateInstance( extensions, extensionsCount );

    SDL_Vulkan_CreateSurface( pWindow, m_instance->GetHandle(), &surface );
    SDL_GetWindowSize( pWindow, &m_width, &m_height );

    m_instance->SetSurface( surface );
    m_instance->InitializeDevice( &m_device, m_instance->FindBestPhysicalDeviceIndex( surface ) );

    m_queue       = FawnVision::CreateQueue( m_device );
    m_swapchain   = FawnVision::CreateSwapchain( m_device, surface, m_width, m_height );
    m_commandPool = FawnVision::CreateCommandPool( m_device, m_swapchain, m_queue->GetQueFamily() );

    m_swapchain->GetImages( m_swapchainResources, imageResourceSize );
    m_swapchainViews.reserve( imageResourceSize );
    for ( uint32_t i = 0; i < imageResourceSize; ++i )
    {
        const FawnVision::SImageViewCreateInfo createInfo {
            .componentMapping = ( (uint32_t)component_swizzle::swizzle_r ) | ( (uint32_t)component_swizzle::swizzle_g << 8 ) | ( (uint32_t)component_swizzle::swizzle_b << 16 ) | ( (uint32_t)component_swizzle::swizzle_a << 24 ),
            .imageViewType    = (uint8_t)FawnVision::image_view_type::view_2d,
        };
        m_swapchainViews.push_back( FawnVision::CreateImageView( m_device, m_swapchainResources[ i ], createInfo ) );
    }
    formats.push_back( (FawnVision::image_format)m_swapchainResources[ 0 ]->GetFormat() );
    formats.push_back( (FawnVision::image_format)FawnVision::ConvertImageFormat( (uint32_t)m_device->GetPhysicalDeviceInfo().GetDepthFormat() ) );

    FawnVision::SImageCreateInfo depthImageCreateInfo {
        .depth         = 1,
        .height        = m_swapchain->GetExtend().height,
        .initialLayout = (uint32_t)FawnVision::image_layout::undefined,
        .usage         = (uint32_t)FawnVision::image_usage_flag_depth_stencil_attachment,
        .width         = m_swapchain->GetExtend().width,
        .format        = (uint8_t)formats.back(),
        .imageType     = (uint8_t)FawnVision::image_view_type::view_2d,
    };

    m_renderPass     = FawnVision::CreateRenderPass( m_device, formats, 0, (uint32_t)m_swapchainViews.size() );
    m_depthImage     = FawnVision::CreateImageResource( m_device, depthImageCreateInfo );
    m_depthImageView = FawnVision::CreateImageView( m_device, m_depthImage, depthViewInfo );
    m_frameBuffer    = FawnVision::CreateFrameBuffer( m_device, m_renderPass, m_width, m_height, m_swapchainViews, m_depthImageView );

    const uint32_t size { m_commandPool->GetCommandBufferCount() };
    m_signalingSemaphore = FawnVision::CreateSemaphore( m_device );
    m_waitingSemaphore   = FawnVision::CreateSemaphore( m_device );
    m_fences.reserve( size );
    m_inFlightFences.reserve( size );
    for ( uint32_t i = 0; i < size; ++i )
    {
        m_fences.push_back( FawnVision::CreateFence( m_device ) );
        m_inFlightFences.push_back( BALBINO_NULL );
    }

    CreateModelBuffer();
    CreateLightBuffer();
    CreateInstanceBuffer();
}
void FawnVision::CRenderer::Cleanup()
{
    m_device->WaitIdle();
    FreeSemaphore( m_signalingSemaphore );
    FreeSemaphore( m_waitingSemaphore );
    if ( m_commandPool )
    {
        const uint64_t size = m_commandPool->GetCommandBufferCount();
        for ( uint64_t i = 0; i < size; ++i )
        {
            FreeFence( m_fences[ i ] );
        }
        if ( !m_fences.empty() )
        {
            m_fences.clear();
        }
    }
    m_pInstanceBuffer->Release();
    m_pLightBuffer->Release();
    m_pModelBuffer->Release();

    FreeRenderPass( m_renderPass );
    for ( auto& image : m_swapchainViews )
    {
        FreeImageView( image );
    }
    for ( auto& image : m_swapchainResources )
    {
        FreeImageResource( image );
    }
    m_swapchainViews.clear();
    m_swapchainResources.clear();
    FreeImageView( m_depthImageView );
    FreeImageResource( m_depthImage );
    FreeFrameBuffer( m_frameBuffer );
    FreeCommandPool( m_commandPool );
    FreeQueue( m_queue );
    FreeSwapchain( m_swapchain );
    FreeDevice( m_device );
    FreeInstance( m_instance );
}
bool FawnVision::CRenderer::StartDraw() noexcept
{
    m_fences[ m_commandPool->GetCurrentIndex() ]->Wait();
    if ( m_swapchain->AcquireNextImage( m_signalingSemaphore, m_imageIndex ) )
    {
        RecreateSwapChain();
        return true;
    }
    m_commandPool->BeginRender( m_renderPass, m_frameBuffer, m_swapchain );
    return false;
}
bool FawnVision::CRenderer::EndDraw() noexcept
{
    if ( m_inFlightFences[ m_imageIndex ] != BALBINO_NULL )
    {
        m_inFlightFences[ m_imageIndex ]->Wait();
    }
    m_inFlightFences[ m_imageIndex ] = m_fences[ m_commandPool->GetCurrentIndex() ];
    m_fences[ m_commandPool->GetCurrentIndex() ]->Reset();
    m_commandPool->EndRender();
    m_queue->SubmitPass( m_waitingSemaphore, m_signalingSemaphore, m_commandPool, m_fences[ m_commandPool->GetCurrentIndex() ] );
    if ( m_queue->PresentToScreen( m_swapchain, m_waitingSemaphore, m_imageIndex ) )
    {
        RecreateSwapChain();
        return true;
    }
    m_queue->WaitIdle();
    m_commandPool->UpdateFrameIndex();

    return false;
}
void FawnVision::CRenderer::RecreateSwapChain() noexcept
{
    SDL_GetWindowSizeInPixels( m_pWindow, &m_width, &m_height );
    while ( m_width == 0 || m_height == 0 )
    {
        SDL_GetWindowSize( m_pWindow, &m_width, &m_height );
    }

    m_device->WaitIdle();
    m_pInstanceBuffer->Release();
    m_pLightBuffer->Release();
    m_pModelBuffer->Release();
    FreeRenderPass( m_renderPass );
    for ( auto& image : m_swapchainViews )
    {
        FreeImageView( image );
    }
    for ( auto& image : m_swapchainResources )
    {
        FreeImageResource( image );
    }
    m_swapchainViews.clear();
    m_swapchainResources.clear();
    FreeImageView( m_depthImageView );
    FreeImageResource( m_depthImage );
    FreeFrameBuffer( m_frameBuffer );
    FreeCommandPool( m_commandPool );
    FreeQueue( m_queue );
    m_swapchain->Initialize( m_width, m_height );

    uint32_t imageResourceSize;
    std::vector<FawnVision::image_format> formats;
    const FawnVision::SImageViewCreateInfo depthViewInfo {
        .componentMapping = ( (uint32_t)component_swizzle::swizzle_r ) | ( (uint32_t)component_swizzle::swizzle_g << 8 ) | ( (uint32_t)component_swizzle::swizzle_b << 16 ) | ( (uint32_t)component_swizzle::swizzle_a << 24 ),
        .imageViewType    = (uint8_t)FawnVision::image_view_type::view_2d,
    };

    m_queue       = FawnVision::CreateQueue( m_device );
    m_commandPool = FawnVision::CreateCommandPool( m_device, m_swapchain, m_queue->GetQueFamily() );

    m_swapchain->GetImages( m_swapchainResources, imageResourceSize );
    m_swapchainViews.reserve( imageResourceSize );
    for ( uint32_t i = 0; i < imageResourceSize; ++i )
    {
        const FawnVision::SImageViewCreateInfo createInfo {
            .componentMapping = ( (uint32_t)component_swizzle::swizzle_r ) | ( (uint32_t)component_swizzle::swizzle_g << 8 ) | ( (uint32_t)component_swizzle::swizzle_b << 16 ) | ( (uint32_t)component_swizzle::swizzle_a << 24 ),
            .imageViewType    = (uint8_t)FawnVision::image_view_type::view_2d,
        };
        m_swapchainViews.push_back( FawnVision::CreateImageView( m_device, m_swapchainResources[ i ], createInfo ) );
    }
    formats.push_back( (FawnVision::image_format)m_swapchainResources[ 0 ]->GetFormat() );
    formats.push_back( (FawnVision::image_format)FawnVision::ConvertImageFormat( (uint32_t)m_device->GetPhysicalDeviceInfo().GetDepthFormat() ) );

    FawnVision::SImageCreateInfo depthImageCreateInfo {
        .depth         = 1,
        .height        = m_swapchain->GetExtend().height,
        .initialLayout = (uint32_t)FawnVision::image_layout::undefined,
        .usage         = (uint32_t)FawnVision::image_usage_flag_depth_stencil_attachment,
        .width         = m_swapchain->GetExtend().width,
        .format        = (uint8_t)formats.back(),
        .imageType     = (uint8_t)FawnVision::image_view_type::view_2d,
    };

    m_renderPass     = FawnVision::CreateRenderPass( m_device, formats, 0, (uint32_t)m_swapchainViews.size() );
    m_depthImage     = FawnVision::CreateImageResource( m_device, depthImageCreateInfo );
    m_depthImageView = FawnVision::CreateImageView( m_device, m_depthImage, depthViewInfo );
    m_frameBuffer    = FawnVision::CreateFrameBuffer( m_device, m_renderPass, m_width, m_height, m_swapchainViews, m_depthImageView );

    CreateModelBuffer();
    CreateLightBuffer();
    CreateInstanceBuffer();
}
void FawnVision::CRenderer::DrawMesh( uint32_t indexCount, uint32_t firstIndex, uint32_t instanceCount, uint32_t firstInstance )
{
    FawnVision::DrawMesh( m_commandPool, indexCount, firstIndex, firstInstance, instanceCount );
}

FawnVision::CTexture* FawnVision::CRenderer::CreateTexture( const CUuid& id, FawnVision::STextureCreateInfo createInfo, bool generateMips, uint32_t type )
{
    FawnVision::CTexture* pTexture { new FawnVision::CTexture { m_device, id } };
    pTexture->Initialize( createInfo, m_commandPool, m_queue, generateMips );
    return (FawnVision::CTexture*)( m_assetMap[ type ][ id ] = (void*)pTexture );
}
FawnVision::CMaterial* FawnVision::CRenderer::CreateMaterial( const CUuid& id, FawnVision::ShaderPipeline pipeline, const FawnVision::SMaterialCreateInfo& createInfo, uint32_t type )
{
    FawnVision::CMaterial* pMaterial { new FawnVision::CMaterial { id, m_commandPool } };
    pMaterial->Initialize( pipeline, createInfo, m_device );
    return (FawnVision::CMaterial*)( m_assetMap[ type ][ id ] = (void*)pMaterial );
}
FawnVision::CMesh* FawnVision::CRenderer::CreateMesh( const CUuid& id, const FawnVision::SMeshCreateInfo& createInfo, uint32_t type )
{
    FawnVision::CMesh* pMesh { new FawnVision::CMesh { createInfo, id } };
    return (FawnVision::CMesh*)( m_assetMap[ type ][ id ] = (void*)pMesh );
}

void FawnVision::CRenderer::UpdateModelBuffer( SModelObject object, size_t size )
{
    m_pModelBuffer->UpdateData( &object, size );
}
void FawnVision::CRenderer::UpdateLightObject( SLightObject object, size_t size )
{
    m_pLightBuffer->UpdateData( &object, size );
}
void FawnVision::CRenderer::PrepareDraw( const std::pair<BalbinoComponent::CTransformComponent, BalbinoComponent::CCameraComponent>& camera, const std::vector<std::pair<BalbinoComponent::CTransformComponent, BalbinoComponent::CLightComponent>>& lights,
                                         const std::vector<std::tuple<BalbinoComponent::CTransformComponent, BalbinoComponent::CMeshRenderComponent, std::vector<SMeshMetadata>>>& meshes )
{
    ProcessCamera( camera );
    ProcessLights( lights );
    ProcessMeshes( meshes );
}
void FawnVision::CRenderer::CreateModelBuffer() noexcept
{
    m_pModelBuffer = FawnVision::CreateBuffer( m_device, m_commandPool, m_queue, sizeof( SModelObject ), FawnVision::buffer_usage_flag_uniform_buffer, FawnVision::memory_property_flag_host_visible | FawnVision::memory_property_flag_host_coherent );
}
void FawnVision::CRenderer::CreateLightBuffer() noexcept
{
    m_pLightBuffer = FawnVision::CreateBuffer( m_device, m_commandPool, m_queue, sizeof( SLightObject ), FawnVision::buffer_usage_flag_uniform_buffer, FawnVision::memory_property_flag_host_visible | FawnVision::memory_property_flag_host_coherent );
}
void FawnVision::CRenderer::CreateInstanceBuffer() noexcept
{
    m_pInstanceBuffer = FawnVision::CreateBuffer( m_device, m_commandPool, m_queue, MAX_INSTANCE_COUNT * sizeof( SInstanceBatch ), FawnVision::buffer_usage_flag_transfer_dst | FawnVision::buffer_usage_flag_vertex_buffer,
                                                  FawnVision::memory_property_flag_host_visible | FawnVision::memory_property_flag_device_local );
}
void FawnVision::CRenderer::ProcessCamera( const std::pair<BalbinoComponent::CTransformComponent, BalbinoComponent::CCameraComponent>& camera )
{
    const auto& [ transformComponent, cameraComponent ] = camera;
    m_camera.UpdateFrameBuffer( (float)m_width, (float)m_height, cameraComponent.GetFov(), cameraComponent.GetNearClip(), cameraComponent.GetFarClip() );
    m_camera.UpdateMatrices( transformComponent.GetTranslation(), transformComponent.GetRotation() );

    SModelObject ubo {
        .view    = m_camera.GetView(),
        .proj    = m_camera.GetProjection(),
        .viewPos = m_camera.GetViewPosition(),
    };
    UpdateModelBuffer( ubo, sizeof( SModelObject ) );
}
void FawnVision::CRenderer::ProcessLights( const std::vector<std::pair<BalbinoComponent::CTransformComponent, BalbinoComponent::CLightComponent>>& lights )
{
    // todo: do proper light culling
    uint32_t id {};
    for ( const auto& [ transform, light ] : lights )
    {
        this->m_lightObject.lights[ id ].type      = (int)light.GetType();
        this->m_lightObject.lights[ id ].position  = transform.GetTranslation();
        this->m_lightObject.lights[ id ].direction = BambiMath::Rotate( transform.GetRotation(), { 0, 0, 1 } );
        this->m_lightObject.lights[ id ].color     = light.GetColor() * light.GetStrength();
        this->m_lightObject.lights[ id ].size      = light.GetSize();
        ++id;
        if ( id == LIGHT_COUNT )
        {
            break;
        }
    }
    if ( id < LIGHT_COUNT - 1 )
    {
        this->m_lightObject.lights[ id ] = SLight { 255u, -1, { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 } };
    }
}
void FawnVision::CRenderer::ProcessMeshes( const std::vector<std::tuple<BalbinoComponent::CTransformComponent, BalbinoComponent::CMeshRenderComponent, std::vector<SMeshMetadata>>>& meshes )
{
    // todo:CPU voxel Frustum culling
    // todo:GPU Frustum/Occlusion culling
    this->m_allDrawableObjects.clear();
    this->m_instanceData.clear();
    uint32_t count {};
    for ( auto& [ transform, mesh, materialData ] : meshes )
    {
        const std::vector<CUuid>& materialsIds = mesh.GetMaterials();
        const CUuid meshId                     = mesh.GetMeshId();

        const uint32_t size { (uint32_t)materialsIds.size() };
        for ( uint32_t i {}; i < size; i++ )
        {
            //compare the mesh and material with the end of the vector of draws
            const CUuid& currentMaterial { materialsIds[ i ] };
            const uint32_t firstIndex { materialData[ i ].firstIndex };

            if ( const auto it { std::find_if( m_allDrawableObjects.cbegin(), m_allDrawableObjects.cend(),
                                               [ firstIndex, currentMaterial, meshId ]( const SIndirectBatch& obj ) {
                                                   return obj.firstIndex == firstIndex && obj.material == currentMaterial && obj.mesh == meshId;
                                               } ) };
                 it != m_allDrawableObjects.cend() )
            {
                //all matches, count
                const uint32_t index { static_cast<uint32_t>( it - this->m_allDrawableObjects.cbegin() ) };
                this->m_allDrawableObjects[ index ].instanceCount++;
                this->m_instanceData[ index ].emplace_back( transform.GetWorldTranslation(), transform.GetWorldRotation(), transform.GetWorldScale() );
            }
            else
            {
                //new draw
                SIndirectBatch newDraw { .mesh = meshId, .material = currentMaterial, .firstInstance = 0, .instanceCount = 1, .firstIndex = materialData[ i ].firstIndex, .indexCount = materialData[ i ].indexCount };
                this->m_allDrawableObjects.push_back( newDraw );
                this->m_instanceData.push_back( { FawnVision::SInstanceBatch { transform.GetWorldTranslation(), transform.GetWorldRotation(), transform.GetWorldScale() } } );
            }
            ++count;
        }
    }
}
void FawnVision::CRenderer::ExecuteRenderPass( int index )
{
    (void)index;

    for ( uint32_t i {}; i < m_allDrawableObjects.size(); ++i )
    {
        UpdateInstanceObject( m_instanceData[ i ] );
        GetAsset<FawnVision::CMesh>( m_allDrawableObjects[ i ].mesh )->Bind();
        m_pInstanceBuffer->Bind( false, true );
        FawnVision::CMaterial* pMaterial { GetAsset<FawnVision::CMaterial>( m_allDrawableObjects[ i ].material ) };
        if ( pMaterial )
        {
            pMaterial->Bind();
            DrawMesh( m_allDrawableObjects[ i ].indexCount, m_allDrawableObjects[ i ].firstIndex, m_allDrawableObjects[ i ].instanceCount, m_allDrawableObjects[ i ].firstInstance );
        }
    }
}
void FawnVision::CRenderer::UpdateInstanceObject( const std::vector<FawnVision::SInstanceBatch>& instanceData )
{
    m_pInstanceBuffer->UpdateData( instanceData.data(), ( instanceData.size() < MAX_INSTANCE_COUNT ? instanceData.size() : MAX_INSTANCE_COUNT ) * sizeof( FawnVision::SInstanceBatch ) );
}
FawnVision::ImageResource FawnVision::CRenderer::CreateImage( const FawnVision::SImageCreateInfo& createInfo ) const
{
    return FawnVision::CreateImageResource( m_device, createInfo );
}
FawnVision::ImageView FawnVision::CRenderer::CreateImageView( FawnVision::ImageResource pImage, const FawnVision::SImageViewCreateInfo& createInfo ) const
{
    return FawnVision::CreateImageView( m_device, pImage, createInfo );
}
FawnVision::Sampler FawnVision::CRenderer::CreateImageSampler( const FawnVision::SSamplerCreateInfo& createInfo ) const
{
    return FawnVision::CreateSampler( m_device, createInfo );
}
FawnVision::Buffer FawnVision::CRenderer::CreateBuffer( uint64_t size, FawnVision::buffer_usage_flag bufferUsage, FawnVision::memory_property_flag memoryProperty )
{
    return FawnVision::CreateBuffer( m_device, m_commandPool, m_queue, size, bufferUsage, memoryProperty );
}
FawnVision::Shader FawnVision::CRenderer::CreateShader( const FawnVision::SShaderCreateInfo& createInfo )
{
    return FawnVision::CreateShader( m_device, createInfo );
}
FawnVision::ShaderPipeline FawnVision::CRenderer::CreateShaderPipeline( FawnVision::SShaderPassCreateInfo createInfo, const std::vector<FawnVision::Shader>& shaders )
{
    return FawnVision::CreateShaderPipeline( m_device, createInfo, shaders, m_renderPass, m_swapchain );
}
