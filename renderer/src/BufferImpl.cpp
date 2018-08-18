#include "BufferImpl.h"

using namespace renderer;

Buffer::Buffer(std::shared_ptr<vk::Device> device, const vk::PhysicalDevice& physicalDevice, const vk::CommandPool& cmdPool, const vk::Queue& queue, const BufferDesc& desc, const BufferData& data)
    : mDevicePtr(std::move(device))
    , mCopyCommandPool(cmdPool)
    , mQueue(queue)
{
    vk::BufferCreateInfo bufferInfo;
    bufferInfo.size = data.size;
    
    if(desc.flags & BufferBindFlags::VertexBuffer)
        bufferInfo.usage |= vk::BufferUsageFlagBits::eVertexBuffer;
    if(desc.flags & BufferBindFlags::IndexBuffer)
        bufferInfo.usage |= vk::BufferUsageFlagBits::eIndexBuffer;
    
    if(desc.usage & BufferUsage::TransferDest)
        bufferInfo.usage |= vk::BufferUsageFlagBits::eTransferDst;
    if(desc.usage & BufferUsage::TransferSrc)
        bufferInfo.usage |= vk::BufferUsageFlagBits::eTransferSrc;
    
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;
    //bufferInfo.flags; // The flags parameter is used to configure sparse buffer memory.
    
    mBuffer = mDevicePtr->createBuffer(bufferInfo);
    
    auto findMemoryType = [&physicalDevice](uint32_t typeFilter, const vk::MemoryPropertyFlags properties) -> uint32_t {
        vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
        
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
        {
            if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }
        
        throw std::runtime_error("failed to find suitable memory type!");
    };
    
    /*
     The VkMemoryRequirements struct has three fields:
     
     size:              The size of the required amount of memory in bytes, may differ from bufferInfo.size.
     alignment:         The offset in bytes where the buffer begins in the allocated region of memory, depends
     on bufferInfo.usage and bufferInfo.flags.
     memoryTypeBits:    Bit field of the memory types that are suitable for the buffer.
     */
    vk::MemoryRequirements memoryRequirements = mDevicePtr->getBufferMemoryRequirements(mBuffer);
    
    vk::MemoryPropertyFlags memoryFlags;
    if(desc.usage & renderer::BufferUsage::Staging)
        memoryFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    else
        memoryFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;
    
    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryFlags);
    mMemory = mDevicePtr->allocateMemory(allocInfo);
    
    mDevicePtr->bindBufferMemory(mBuffer, mMemory, 0);
}

void Buffer::CopyData(std::shared_ptr<IBuffer>& srcBuffer, std::shared_ptr<IBuffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size)
{
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = mCopyCommandPool;
    allocInfo.commandBufferCount = 1;
    
    auto commandBuffers = mDevicePtr->allocateCommandBuffers(allocInfo);
    
    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    
    auto& cmdBuffer = commandBuffers.back();
    cmdBuffer.begin(beginInfo);
    
    vk::BufferCopy copyInfo;
    copyInfo.srcOffset = srcOffset; // Optional
    copyInfo.dstOffset = dstOffset; // Optional
    copyInfo.size = size;
    
    auto srcBufferVk = std::static_pointer_cast<Buffer>(srcBuffer);
    auto dstBufferVk = std::static_pointer_cast<Buffer>(dstBuffer);
    
    cmdBuffer.copyBuffer(srcBufferVk->GetVulkanBuffer(), dstBufferVk->GetVulkanBuffer(), { copyInfo });
    cmdBuffer.end();
    
    std::vector<vk::CommandBuffer> cmdBuffers{ cmdBuffer };
    
    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());
    submitInfo.pCommandBuffers = cmdBuffers.data();
    
    mQueue.submit(1, &submitInfo, {});
    mQueue.waitIdle();
    
    // We could use a fence and wait with vkWaitForFences, or simply wait for the transfer queue to become idle with vkQueueWaitIdle. A fence would allow you to schedule multiple
    // transfers simultaneously and wait for all of them complete, instead of executing one at a time. That may give the driver more opportunities to optimize.
    
    mDevicePtr->freeCommandBuffers(mCopyCommandPool, 1, &cmdBuffer);

}
