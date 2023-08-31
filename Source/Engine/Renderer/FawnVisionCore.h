#pragma once
#include <Core.h>
#include <UUID.h>
#include <BambiMath.h>

#include <source_location>
#include <vulkan/vulkan.hpp>

#include "MeshMetadata.h"

#define TYPE_NAME( object )                                                                                                                                                                                                                                    \
    namespace DeerVulkan                                                                                                                                                                                                                                       \
    {                                                                                                                                                                                                                                                          \
        class C##object;                                                                                                                                                                                                                                       \
    }                                                                                                                                                                                                                                                          \
    namespace FawnVision                                                                                                                                                                                                                                       \
    {                                                                                                                                                                                                                                                          \
        using object = DeerVulkan::C##object*;                                                                                                                                                                                                                 \
    }

TYPE_NAME( Buffer )
TYPE_NAME( CommandPool )
TYPE_NAME( DescriptorSet )
TYPE_NAME( Device )
TYPE_NAME( Fence )
TYPE_NAME( FrameBuffer )
TYPE_NAME( ImageResource )
TYPE_NAME( ImageView )
TYPE_NAME( Instance )
TYPE_NAME( Queue )
TYPE_NAME( RenderPass )
TYPE_NAME( Sampler )
TYPE_NAME( Semaphore )
TYPE_NAME( Shader )
TYPE_NAME( ShaderPipeline )
TYPE_NAME( Swapchain )

namespace FawnVision
{
#define SAFE_FREE_DECL( object ) void Free##object( object& o );
#define SAFE_FREE( object )                                                                                                                                                                                                                                    \
    void Free##object( object& o )                                                                                                                                                                                                                             \
    {                                                                                                                                                                                                                                                          \
        if ( o )                                                                                                                                                                                                                                               \
        {                                                                                                                                                                                                                                                      \
            o->Release();                                                                                                                                                                                                                                      \
            o = BALBINO_NULL;                                                                                                                                                                                                                                  \
        }                                                                                                                                                                                                                                                      \
    }

    enum vertex_component_flags : uint8_t
    {
        vertex_component_flag_position = 0x01,
        vertex_component_flag_color    = 0x02,
        vertex_component_flag_uv       = 0x04,
        vertex_component_flag_normal   = 0x08,
        vertex_component_flag_tangent  = 0x10,
        vertex_component_flag_max      = 0x20
    };
    enum memory_property_flag : uint16_t
    {
        memory_property_flag_device_local        = 0x0001,
        memory_property_flag_host_visible        = 0x0002,
        memory_property_flag_host_coherent       = 0x0004,
        memory_property_flag_host_cached         = 0x0008,
        memory_property_flag_lazily_allocated    = 0x0010,
        memory_property_flag_protected           = 0x0020,
        memory_property_flag_device_coherent_amd = 0x0040,
        memory_property_flag_device_uncached_amd = 0x0080,
        memory_property_flag_rdma_capable_nv     = 0x0100,
        memory_property_flag_max                 = 0x01ff
    };
    enum buffer_usage_flag : uint32_t
    {
        buffer_usage_flag_transfer_src                                     = 0x00000001,
        buffer_usage_flag_transfer_dst                                     = 0x00000002,
        buffer_usage_flag_uniform_texel_buffer                             = 0x00000004,
        buffer_usage_flag_storage_texel_buffer                             = 0x00000008,
        buffer_usage_flag_uniform_buffer                                   = 0x00000010,
        buffer_usage_flag_storage_buffer                                   = 0x00000020,
        buffer_usage_flag_index_buffer                                     = 0x00000040,
        buffer_usage_flag_vertex_buffer                                    = 0x00000080,
        buffer_usage_flag_indirect_buffer                                  = 0x00000100,
        buffer_usage_flag_shader_device_address                            = 0x00020000,
        buffer_usage_flag_transform_feedback_buffer_ext                    = 0x00000800,
        buffer_usage_flag_transform_feedback_counter_buffer_ext            = 0x00001000,
        buffer_usage_flag_conditional_rendering_ext                        = 0x00000200,
        buffer_usage_flag_acceleration_structure_build_input_read_only_khr = 0x00080000,
        buffer_usage_flag_acceleration_structure_storage_khr               = 0x00100000,
        buffer_usage_flag_shader_binding_table_khr                         = 0x00000400,
        buffer_usage_flag_ray_tracing_nv                                   = buffer_usage_flag_shader_binding_table_khr,
        buffer_usage_flag_shader_device_address_ext                        = buffer_usage_flag_shader_device_address,
        buffer_usage_flag_shader_device_address_khr                        = buffer_usage_flag_shader_device_address,
        buffer_usage_flag_max                                              = 0x7FFFFFFF
    };
    enum shader_stage : uint16_t
    {
        shader_stage_vertex                  = 1 << 0,
        shader_stage_tessellation_control    = 1 << 1,
        shader_stage_tessellation_evaluation = 1 << 2,
        shader_stage_geometry                = 1 << 3,
        shader_stage_fragment                = 1 << 4,
        shader_stage_compute                 = 1 << 5,
        shader_stage_raygen                  = 1 << 6,
        shader_stage_any_hit                 = 1 << 7,
        shader_stage_closest_hit             = 1 << 8,
        shader_stage_miss                    = 1 << 9,
        shader_stage_intersection            = 1 << 10,
        shader_stage_callable                = 1 << 11,
        shader_stage_task                    = 1 << 12,
        shader_stage_mesh                    = 1 << 13,
        shader_stage_max                     = ( 1 << 14 ) - 1
    };
    enum image_usage_flag : uint32_t
    {
        image_usage_flag_transfer_src                         = 0x00000001,
        image_usage_flag_transfer_dst                         = 0x00000002,
        image_usage_flag_sampled                              = 0x00000004,
        image_usage_flag_storage                              = 0x00000008,
        image_usage_flag_color_attachment                     = 0x00000010,
        image_usage_flag_depth_stencil_attachment             = 0x00000020,
        image_usage_flag_transient_attachment                 = 0x00000040,
        image_usage_flag_input_attachment                     = 0x00000080,
        image_usage_flag_fragment_density_map_bit             = 0x00000200,
        image_usage_flag_fragment_shading_rate_attachment_khr = 0x00000100,
        image_usage_flag_invocation_mask_huawei               = 0x00040000,
        image_usage_flag_shading_rate_image_nv                = image_usage_flag_fragment_shading_rate_attachment_khr,
        image_usage_flag_max                                  = 0x7FFFFFFF
    };
    enum pipeline_stage_flag
    {
        top_of_pipe_bit                          = 0x00000001,
        draw_indirect_bit                        = 0x00000002,
        vertex_input_bit                         = 0x00000004,
        vertex_shader_bit                        = 0x00000008,
        tessellation_control_shader_bit          = 0x00000010,
        tessellation_evaluation_shader_bit       = 0x00000020,
        geometry_shader_bit                      = 0x00000040,
        fragment_shader_bit                      = 0x00000080,
        early_fragment_tests_bit                 = 0x00000100,
        late_fragment_tests_bit                  = 0x00000200,
        color_attachment_output_bit              = 0x00000400,
        compute_shader_bit                       = 0x00000800,
        transfer_bit                             = 0x00001000,
        bottom_of_pipe_bit                       = 0x00002000,
        host_bit                                 = 0x00004000,
        all_graphics_bit                         = 0x00008000,
        all_commands_bit                         = 0x00010000,
        transform_feedback_bit_ext               = 0x01000000,
        conditional_rendering_bit_ext            = 0x00040000,
        acceleration_structure_build_bit_khr     = 0x02000000,
        ray_tracing_shader_bit_khr               = 0x00200000,
        shading_rate_image_bit_nv                = 0x00400000,
        task_shader_bit_nv                       = 0x00080000,
        mesh_shader_bit_nv                       = 0x00100000,
        fragment_density_process_bit_ext         = 0x00800000,
        command_preprocess_bit_nv                = 0x00020000,
        none_khr                                 = 0,
        ray_tracing_shader_bit_nv                = ray_tracing_shader_bit_khr,
        acceleration_structure_build_bit_nv      = acceleration_structure_build_bit_khr,
        fragment_shading_rate_attachment_bit_khr = shading_rate_image_bit_nv,
        flag_bits_max_enum                       = 0x7FFFFFFF
    };

    enum class component_swizzle : uint8_t
    {
        swizzle_identity = 0,
        swizzle_zero     = 1,
        swizzle_one      = 2,
        swizzle_r        = 3,
        swizzle_g        = 4,
        swizzle_b        = 5,
        swizzle_a        = 6,
        max              = 7
    };
    enum class image_layout : uint32_t
    {
        undefined                                      = 0,
        general                                        = 1,
        color_attachment_optimal                       = 2,
        depth_stencil_attachment_optimal               = 3,
        depth_stencil_read_only_optimal                = 4,
        shader_read_only_optimal                       = 5,
        transfer_src_optimal                           = 6,
        transfer_dst_optimal                           = 7,
        preinitialized                                 = 8,
        depth_read_only_stencil_attachment_optimal     = 1000117000,
        depth_attachment_stencil_read_only_optimal     = 1000117001,
        depth_attachment_optimal                       = 1000241000,
        depth_read_only_optimal                        = 1000241001,
        stencil_attachment_optimal                     = 1000241002,
        stencil_read_only_optimal                      = 1000241003,
        present_src_khr                                = 1000001002,
        shared_present_khr                             = 1000111000,
        fragment_density_map_optimal_ext               = 1000218000,
        fragment_shading_rate_attachment_optimal_khr   = 1000164003,
        read_only_optimal_khr                          = 1000314000,
        attachment_optimal_khr                         = 1000314001,
        depth_read_only_stencil_attachment_optimal_khr = depth_read_only_stencil_attachment_optimal,
        depth_attachment_stencil_read_only_optimal_khr = depth_attachment_stencil_read_only_optimal,
        shading_rate_optimal_nv                        = fragment_shading_rate_attachment_optimal_khr,
        depth_attachment_optimal_khr                   = depth_attachment_optimal,
        depth_read_only_optimal_khr                    = depth_read_only_optimal,
        stencil_attachment_optimal_khr                 = stencil_attachment_optimal,
        stencil_read_only_optimal_khr                  = stencil_read_only_optimal,
        max                                            = 0x7FFFFFFF
    };
    enum class image_filter : uint8_t
    {
        nearest = 0,
        linear  = 1,
        max     = 2
    };
    enum class image_format : uint8_t
    {
        undefined = 0,
        r16_g16_b16_a16,
        r8_g8_b8_a8,
        b8_g8_r8_a8,
        r4_g4_b4_a4,
        r16_g16_b16,
        r5_g5_b5_a1,
        r8_g8_b8,
        r5_g6_b5,
        r32_g32,
        r16_g16,
        r8_g8,
        r8,
        r16,
        a8,
        d16_unorm,
        d32_sfloat,
        d16_unorm_s8_uint,
        d24_unorm_s8_uint,
        d32_sfloat_s8_uint,
        max_enum = 20
    };
    enum class image_view_type : uint8_t
    {
        view_1d,
        view_1d_array,
        view_2d,
        view_2d_array,
        view_3d,
        view_rect,
        view_rect_array,
        view_cube,
        view_cube_array,
    };
    enum class sampler_address_mode : uint8_t
    {
        repeat                   = 0,
        mirrored_repeat          = 1,
        clamp_to_edge            = 2,
        clamp_to_border          = 3,
        mirror_clamp_to_edge     = 4,
        mirror_clamp_to_edge_khr = mirror_clamp_to_edge,
        max                      = 5
    };
    enum class compare_operator : uint8_t
    {
        never            = 0,
        less             = 1,
        equal            = 2,
        less_or_equal    = 3,
        greater          = 4,
        not_equal        = 5,
        greater_or_equal = 6,
        always           = 7,
        max              = 8
    };
    enum class cull_mode : uint8_t
    {
        front,
        none,
        back
    };
    enum class border_color : uint8_t
    {
        float_transparent_black = 0,
        int_transparent_black   = 1,
        float_opaque_black      = 2,
        int_opaque_black        = 3,
        float_opaque_white      = 4,
        int_opaque_white        = 5,
        float_custom_ext        = 7,
        int_custom_ext          = 8,
        max                     = 9
    };
    enum class shader_resource_type : uint8_t
    {
        none,
        input,
        input_attachment,
        output,
        image,
        image_sampler,
        image_storage,
        sampler,
        buffer_uniform,
        buffer_storage,
        push_constant,
        specialization_constant,
    };
    enum class shader_resource_mode : uint8_t
    {
        static_mode,
        dynamic_mode,
        update_after_bind_mode
    };

    struct SDescriptorImageInfo final {
        ImageView view { BALBINO_NULL };
        Sampler sampler { BALBINO_NULL };
    };
    struct SDescriptorBufferInfo final {
        Buffer buffer { BALBINO_NULL };
    };
    struct SDescriptorSet final {
        enum class type
        {
            image,
            buffer,
            dynamic_buffer,
            buffer_storage,
            input
        };
        union UDescriptorInfo
        {
            SDescriptorImageInfo image;
            SDescriptorBufferInfo buffer;
        };
        UDescriptorInfo description {};
        uint32_t set {};
        uint32_t binding {};
        type descriptorType {};

        SDescriptorSet() = default;

        SDescriptorSet( type buffer, Buffer modelBuffer, uint32_t set, uint32_t binding )
            : descriptorType { buffer }
            , description { .buffer = { modelBuffer } }
            , set( set )
            , binding { binding }
        {
        }

        SDescriptorSet( type buffer, ImageView imageView, Sampler sampler, uint32_t set, uint32_t binding )
            : descriptorType { buffer }
            , description { .image = { imageView, sampler } }
            , set( set )
            , binding { binding }
        {
        }
    };
    struct SComponentMapping final {
        component_swizzle r { component_swizzle::swizzle_r };
        component_swizzle g { component_swizzle::swizzle_g };
        component_swizzle b { component_swizzle::swizzle_b };
        component_swizzle a { component_swizzle::swizzle_a };
    };
    struct SVertex final {
        BambiMath::Vector3 position { 0, 0, 0 };
        BambiMath::Vector4 color { 1, 1, 1, 1 };
        BambiMath::Vector2 uv { 0, 0 };
        BambiMath::Vector3 normal { 0, 1, 0 };
        BambiMath::Vector4 tangent { 1, 0, 0, 1 };
    };
    struct SShaderResource final {
        std::string name {};
        CUuid resourceId {};
        uint32_t set {};
        uint32_t binding {};
        uint32_t location {};
        uint32_t inputAttachmentIndex {};
        uint32_t vecSize {};
        uint32_t columns {};
        uint32_t arraySize {};
        uint32_t offset {};
        uint32_t size {};
        uint32_t constantId {};
        uint32_t qualifiers {};
        uint16_t stages {};
        uint8_t type {};
        uint8_t mode {};
    };
    struct SImageCreateInfo final {
        uint32_t arrayLayers { 1 };
        uint32_t depth { 1 };
        uint32_t height { 1 };
        uint32_t initialLayout {};
        uint32_t mipLevels { 1 };
        uint32_t samples { 1 };
        uint32_t usage {};
        uint32_t width { 1 };
        uint8_t format { 0 };
        uint8_t imageType {};
    };
    struct SImageViewCreateInfo final {
        uint32_t componentMapping {};
        uint32_t firstLayer { 0 };
        uint32_t firstMip { 0 };
        uint32_t layerCount { 1 };
        uint32_t mipCount { 1 };
        uint8_t imageViewType {};
    };
    struct SSamplerCreateInfo final {
        float maxAnisotropy {};
        float maxLod {};
        float minLod {};
        float mipLodBias {};
        uint32_t compareEnable {};
        uint32_t unNormalizedCoordinates {};
        uint8_t addressModeU {};
        uint8_t addressModeV {};
        uint8_t addressModeW {};
        uint8_t minFilter {};
        uint8_t mipMapMode {};
        uint8_t borderColor {};
        uint8_t compareOperator {};
        uint8_t magFilter {};
    };
    struct SShaderCreateInfo final {
        uint32_t type {};
        uint64_t size {};
        uint32_t* pData { BALBINO_NULL };
    };
    struct SShaderPassCreateInfo final {
        uint32_t blendAttachmentSize { 1 };
        uint32_t types {};
        uint8_t components {};
        uint8_t cullModeFlag {};
    };
    struct SMeshCreateInfo final {
        std::vector<uint32_t> indices {};
        std::vector<SVertex> vertices {};
        std::vector<SMeshMetadata> metadata {};
    };
    struct SMaterialCreateInfo final {
        CUuid shaderId {};
        std::vector<SShaderResource> shaderResources {};
        std::vector<SDescriptorSet> descriptorSets {};
    };
    struct STextureCreateInfo final {
        SImageCreateInfo imageCreateInfo {};
        SImageViewCreateInfo imageViewCreateInfo {};
        SSamplerCreateInfo samplerCreateInfo {};
        uint32_t imageDataSize {};
        void* pImageData { BALBINO_NULL };
    };
    struct SInstanceBatch final {
        SInstanceBatch() = default;
        constexpr explicit SInstanceBatch( const BambiMath::Vector2& trans, const BambiMath::Quaternion& quat, BambiMath::Vector3 sca )
            : transform { trans }
            , rotation { quat }
            , scale { sca }
        {
        }
        BambiMath::Vector2 transform;
        BambiMath::Quaternion rotation;
        BambiMath::Vector2 scale;
    };

    void DrawMesh( const CommandPool& command, uint32_t indexCount, uint32_t firstIndex = 0, uint32_t firstInstance = 0, uint32_t instanceCount = 1 );
    bool IsFormatSupported( image_format format );

    Buffer CreateBuffer( Device device, CommandPool commandPool, Queue queue, uint64_t size, uint32_t usageFlags, uint16_t memoryPropertyFlags );
    CommandPool CreateCommandPool( Device device, Swapchain swapchain, uint32_t index );
    DescriptorSet CreateDescriptorSet( Device device, ShaderPipeline shaderPipeline, const std::vector<SDescriptorSet>& descriptorSets );
    // Device CreateDevice(const SPhysicalDeviceInfo& physicalDeviceInfo, const std::vector<const char*>& layersToEnable, const std::vector<const char*>& extensionsToEnable);
    Fence CreateFence( Device device );
    FrameBuffer CreateFrameBuffer( Device device, RenderPass renderPass, uint32_t width, uint32_t height, const std::vector<ImageView>& renderTargets, ImageView depth );
    ImageResource CreateImageResource( Device device, const SImageCreateInfo& createInfo );
    ImageView CreateImageView( Device device, ImageResource resource, const SImageViewCreateInfo& createInfo );
    Instance CreateInstance( const char** extensions, uint32_t extensionsCount );
    Queue CreateQueue( Device device );
    RenderPass CreateRenderPass( Device device, const std::vector<image_format>& formats, uint32_t inputCount, bool hasColor );
    Sampler CreateSampler( Device device, const SSamplerCreateInfo& createInfo );
    Semaphore CreateSemaphore( Device device );
    Shader CreateShader( Device device, const SShaderCreateInfo& createInfo );
    ShaderPipeline CreateShaderPipeline( Device device, const SShaderPassCreateInfo& createInfo, const std::vector<Shader>& shaders, RenderPass renderPass, Swapchain swapchain );
    Swapchain CreateSwapchain( Device device, const VkSurfaceKHR& surface, uint32_t width, uint32_t height );

    SAFE_FREE_DECL( Buffer );
    SAFE_FREE_DECL( CommandPool );
    SAFE_FREE_DECL( DescriptorSet );
    SAFE_FREE_DECL( Device );
    SAFE_FREE_DECL( Fence );
    SAFE_FREE_DECL( FrameBuffer );
    SAFE_FREE_DECL( ImageResource );
    SAFE_FREE_DECL( ImageView );
    SAFE_FREE_DECL( Queue );
    SAFE_FREE_DECL( RenderPass );
    SAFE_FREE_DECL( Sampler );
    SAFE_FREE_DECL( Semaphore );
    SAFE_FREE_DECL( Shader );
    SAFE_FREE_DECL( ShaderPipeline );
    SAFE_FREE_DECL( Swapchain );
    SAFE_FREE_DECL( Instance );

    template<typename T>
    inline VkVertexInputBindingDescription GetBindingDescription();
    template<typename T>
    inline std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions( uint32_t binding, FawnVision::vertex_component_flags components );
    template<typename T>
    inline std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
}// namespace FawnVision
