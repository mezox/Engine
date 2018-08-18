#pragma once

#include "Buffer.h"
#include "Vertex.h"
#include <vulkan/vulkan.hpp>

#include <memory>

namespace renderer
{
    class Buffer : public IBuffer
    {
        DISALLOW_COPY_AND_ASSIGN(Buffer)
        
    public:
        Buffer() = default;
        Buffer(std::shared_ptr<vk::Device> device,  const vk::PhysicalDevice& physicalDevice, const vk::CommandPool& cmdPool, const vk::Queue& queue, const BufferDesc& desc, const BufferData& data);
        
        void CopyData(std::shared_ptr<IBuffer>& srcBuffer, std::shared_ptr<IBuffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size) override final;
        
        const vk::Buffer& GetVulkanBuffer() const { return mBuffer; }
        const vk::DeviceMemory& GetVulkanMemory() const { return mMemory; }
        
    private:
        std::shared_ptr<vk::Device> mDevicePtr;
        vk::Buffer mBuffer;
        vk::DeviceMemory mMemory;
        vk::CommandPool mCopyCommandPool;
        vk::Queue mQueue;
    };
}
