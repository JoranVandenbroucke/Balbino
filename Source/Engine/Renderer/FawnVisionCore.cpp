//
// Created by joran on 28/07/2023.
//
#include "FawnVisionCore.h"
#include <Buffer.h>
#include <CommandPool.h>
#include <DescriptorSet.h>
#include <Device.h>
#include <Fence.h>
#include <FrameBuffer.h>
#include <ImageResource.h>
#include <ImageView.h>
#include <Instance.h>
#include <Queue.h>
#include <RenderPass.h>
#include <Sampler.h>
#include <Semaphore.h>
#include <Shader.h>
#include <ShaderPipeline.h>
#include <Swapchain.h>

namespace FawnVision
{
    void DrawMesh(CommandPool const& command, uint32_t indexCount, uint32_t firstIndex, uint32_t firstInstance, uint32_t instanceCount)
    {
        vkCmdDrawIndexed(command->GetCommandBuffer(), indexCount, instanceCount, firstIndex, 0, firstInstance);
    }
    bool IsFormatSupported(const VkPhysicalDevice& physicalDevice, image_format format)
    {
        VkFormatProperties formatPropertiesList;
        vkGetPhysicalDeviceFormatProperties( physicalDevice, (VkFormat)ConvertImageFormat( format ), &formatPropertiesList );

        if ( formatPropertiesList.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT || formatPropertiesList.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT )
        {
            return true;
        }
        return false;
    }

    Buffer CreateBuffer(Device device, CommandPool commandPool, Queue queue, uint64_t size, uint32_t usageFlags, uint16_t memoryPropertyFlags)
    {
        Buffer buffer { new DeerVulkan::CBuffer { device, commandPool, queue }};
        buffer->Initialize(size, usageFlags, memoryPropertyFlags);
        return buffer;
    }
    CommandPool CreateCommandPool(Device device, Swapchain swapchain, uint32_t index)
    {
        CommandPool commandPool { new DeerVulkan::CCommandPool { device }};
        commandPool->Initialize(index, swapchain);
        return commandPool;
    }
    DescriptorSet CreateDescriptorSet(Device device, ShaderPipeline shaderPipeline, const std::vector<SDescriptorSet>& descriptorSets)
    {
        DescriptorSet descriptorSet { new DeerVulkan::CDescriptorSet { device }};
        descriptorSet->Initialize(shaderPipeline, descriptorSets);
        return descriptorSet;
    }
    //    Device CreateDevice(const SPhysicalDeviceInfo& physicalDeviceInfo, const std::vector<const char*>& layersToEnable, const std::vector<const char*>& extensionsToEnable)
    //    {
    //        Device device { new CDevice { physicalDeviceInfo }};
    //        device->Initialize(layersToEnable, extensionsToEnable);
    //        return device;
    //    }
    Fence CreateFence(Device device)
    {
        Fence fence { new DeerVulkan::CFence { device }};
        fence->Initialize();
        return fence;
    }
    FrameBuffer CreateFrameBuffer(Device device, FawnVision::RenderPass renderPass, uint32_t width, uint32_t height, const std::vector<FawnVision::ImageView>& renderTargets, ImageView depth)
    {
        FrameBuffer frameBuffer { new DeerVulkan::CFrameBuffer { device }};
        frameBuffer->Initialize(renderPass, width, height, renderTargets, depth);
        return frameBuffer;
    }
    ImageResource CreateImageResource(Device device, const FawnVision::SImageCreateInfo& createInfo)
    {
        ImageResource imageResource { new DeerVulkan::CImageResource { device }};
        imageResource->Initialize(createInfo);
        return imageResource;
    }
    ImageView CreateImageView(Device device, ImageResource resource, const FawnVision::SImageViewCreateInfo& createInfo)
    {
        ImageView imageView { new DeerVulkan::CImageView { device }};
        imageView->Initialize(createInfo, resource);
        return imageView;
    }
    Instance CreateInstance(const char** extensions, uint32_t extensionsCount)
    {
        Instance instance { new DeerVulkan::CInstance {}};
        instance->Initialize(extensions, extensionsCount);
        return instance;
    }
    Queue CreateQueue(Device device)
    {
        Queue queue { new DeerVulkan::CQueue { device }};
        queue->Initialize();
        return queue;
    }
    RenderPass CreateRenderPass(Device device, const std::vector<FawnVision::image_format>& formats, uint32_t inputCount, bool hasColor)
    {
        RenderPass renderPass { new DeerVulkan::CRenderPass { device }};
        renderPass->Initialize(formats, inputCount, hasColor);
        return renderPass;
    }
    Sampler CreateSampler(Device device, const FawnVision::SSamplerCreateInfo& createInfo)
    {
        Sampler sampler { new DeerVulkan::CSampler { device }};
        sampler->Initialize(createInfo);
        return sampler;
    }
    Semaphore CreateSemaphore(Device device)
    {
        Semaphore semaphore { new DeerVulkan::CSemaphore { device }};
        semaphore->Initialize();
        return semaphore;
    }
    Shader CreateShader(Device device, const FawnVision::SShaderCreateInfo& createInfo)
    {
        Shader shader { new DeerVulkan::CShader { device }};
        shader->Initialize(createInfo);
        return shader;
    }
    ShaderPipeline CreateShaderPipeline(Device device, const FawnVision::SShaderPassCreateInfo& createInfo, const std::vector<Shader>& shaders, RenderPass renderPass, Swapchain swapchain)
    {
        ShaderPipeline shaderPipeline { new DeerVulkan::CShaderPipeline { device }};
        shaderPipeline->Initialize(createInfo, shaders, renderPass, swapchain);
        return shaderPipeline;
    }
    Swapchain CreateSwapchain(Device device, const VkSurfaceKHR& surface, uint32_t width, uint32_t height)
    {
        Swapchain swapchain { new DeerVulkan::CSwapchain { device, surface }};
        swapchain->Initialize(width, height);
        return swapchain;
    }

    SAFE_FREE(Buffer);
    SAFE_FREE(CommandPool);
    SAFE_FREE(DescriptorSet);
    SAFE_FREE(Device);
    SAFE_FREE(Fence);
    SAFE_FREE(FrameBuffer);
    SAFE_FREE(ImageResource);
    SAFE_FREE(ImageView);
    SAFE_FREE(Queue);
    SAFE_FREE(RenderPass);
    SAFE_FREE(Sampler);
    SAFE_FREE(Semaphore);
    SAFE_FREE(Shader);
    SAFE_FREE(ShaderPipeline);
    SAFE_FREE(Swapchain);
    void FreeInstance(Instance& o)
    {
        if ( o )
        {
            delete o;
            o = BALBINO_NULL;
        }
    }

}