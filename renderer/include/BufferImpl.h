#pragma once

#include "Buffer.h"
#include "Vertex.h"
#include <vulkan/vulkan_core.h>

#include <memory>

namespace renderer
{
    class Buffer : public IBuffer
    {
        DISALLOW_COPY_AND_ASSIGN(Buffer)
        
    public:
        Buffer() = default;
        Buffer(const VkCommandPool& cmdPool, const VkQueue& queue, const BufferDesc& desc, const BufferData& data);
		~Buffer();
        
        void CopyData(std::shared_ptr<IBuffer>& srcBuffer, std::shared_ptr<IBuffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size) override final;
		void Map(const uint64_t offset, const uint64_t size, void* data) override final;
		void Release() override final;
        
        const VkBuffer& GetVulkanBuffer() const { return mBuffer; }
        const VkDeviceMemory& GetVulkanMemory() const { return mMemory; }
        
    private:
        VkBuffer mBuffer;
        VkDeviceMemory mMemory;
        VkCommandPool mCopyCommandPool;
        VkQueue mQueue;
    };
}
