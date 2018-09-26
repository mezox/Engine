#pragma once

#include "RenderAPI.h"

#include <vulkan/vulkan.h> // Wrap vulkan objects and get rid of this

namespace Engine
{
    std::shared_ptr<IRenderAPI> CreateVulkanRenderBackEnd();
    namespace Utilities
    {
        const uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties);
        VkDeviceQueueCreateInfo CreateQueues(const uint32_t familyIndex, const std::vector<float>& priorities);
    }
    
    class VulkanRenderAPI : public IRenderAPI
    {
    public:
        VulkanRenderAPI();
        
        // IRenderAPI
        virtual void Initialize(void* window) override;
        virtual void CreateShader(std::shared_ptr<Renderer::RendererEffect>& gpuEffect, const std::vector<char>& sourceCode) override;
        virtual void CreateTexture(Renderer::Texture& texture) override;
        virtual void CreateBuffer(const Renderer::BufferDesc& desc, const Renderer::BufferData& data, std::shared_ptr<Renderer::Buffer>& bufferObject) override;
        virtual void CopyBuffer(std::shared_ptr<Renderer::Buffer>& srcBuffer, std::shared_ptr<Renderer::Buffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size) override;
        virtual void CreateDescriptorSetLayout() override;
        virtual void CreatePipeline(Renderer::Effect& effect) override;
        
        virtual void Deinitialize() override;
        
        const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return mLayout; }
        VkCommandPool GetPool() const { return *mCmdPool.get(); }
        VkQueue GetGraphicsQueue() const { return *mGraphicsQueue.get(); }
        VkDevice GetDevice() const { return *mDevice.get(); }
        
    private:
        void TransitionImageLayout(VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyBufferToImage(const VkBuffer& buf, const VkImage& image, const uint32_t width, const uint32_t height);
        VkImageView CreateImageView(const VkImage& image, VkFormat format, VkImageAspectFlagBits aspect);
        VkSampler CreateTextureSampler();
        
    private:
        std::shared_ptr<VkDevice> mDevice;
        std::shared_ptr<VkPhysicalDevice> mPhysicalDevice;
        std::shared_ptr<VkCommandPool> mCmdPool;
        std::shared_ptr<VkQueue> mGraphicsQueue;
        
        VkDescriptorSetLayout mLayout;
    };
}
