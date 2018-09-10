#include "BufferImpl.h"

#include <iostream>
#include <PAL/Graphics/LowVK.h>

using namespace renderer;

Buffer::Buffer(const VkCommandPool& cmdPool, const VkQueue& queue, const BufferDesc& desc, const BufferData& data)
    : mCopyCommandPool(cmdPool)
    , mQueue(queue)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = data.size;
    
    if(desc.flags & BufferBindFlags::VertexBuffer)
        bufferInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if(desc.flags & BufferBindFlags::IndexBuffer)
        bufferInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	if (desc.flags & BufferBindFlags::UniformBuffer)
		bufferInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    
    if(desc.usage & BufferUsage::TransferDest)
        bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    if(desc.usage & BufferUsage::TransferSrc) 
        bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    //bufferInfo.flags; // The flags parameter is used to configure sparse buffer memory.
    
	LowVK::CreateBuffer(&bufferInfo, nullptr, &mBuffer);

	auto physicalDevicePtr = LowVK::GetPhysical();
    
    auto findMemoryType = [](uint32_t typeFilter, const VkMemoryPropertyFlags properties) -> uint32_t {
		VkPhysicalDeviceMemoryProperties memoryProperties;
		
		LowVK::GetPhysicalDeviceMemoryProperties(&memoryProperties);
        
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
	VkMemoryRequirements memoryRequirements;
	LowVK::GetBufferMemoryRequirements(mBuffer, &memoryRequirements);
    
    VkMemoryPropertyFlags memoryFlags;
    if(desc.usage & renderer::BufferUsage::Staging)
        memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    else
        memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryFlags);

	LowVK::AllocateMemory(&allocInfo, nullptr, &mMemory);
	LowVK::BindBufferMemory(mBuffer, mMemory, 0);
}

Buffer::~Buffer()
{
}

void Buffer::CopyData(std::shared_ptr<IBuffer>& srcBuffer, std::shared_ptr<IBuffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = mCopyCommandPool;
    allocInfo.commandBufferCount = 1;

	VkCommandBuffer cmdBuffer;
	LowVK::AllocateCommandBuffers(&allocInfo, &cmdBuffer);
    
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	LowVK::BeginCommandBuffer(cmdBuffer, &beginInfo);
    
	VkBufferCopy copyInfo;
    copyInfo.srcOffset = srcOffset; // Optional
    copyInfo.dstOffset = dstOffset; // Optional
    copyInfo.size = size;
    
    auto srcBufferVk = std::static_pointer_cast<Buffer>(srcBuffer);
    auto dstBufferVk = std::static_pointer_cast<Buffer>(dstBuffer);
    
	LowVK::CmdCopyBuffer(cmdBuffer, srcBufferVk->GetVulkanBuffer(), dstBufferVk->GetVulkanBuffer(), { copyInfo });

    LowVK::EndCommandBuffer(cmdBuffer);
    
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;
    
	LowVK::QueueSubmit(mQueue, { submitInfo }, {});
	LowVK::QueueWaitIdle(mQueue);
    
    // We could use a fence and wait with vkWaitForFences, or simply wait for the transfer queue to become idle with vkQueueWaitIdle. A fence would allow you to schedule multiple
    // transfers simultaneously and wait for all of them complete, instead of executing one at a time. That may give the driver more opportunities to optimize.
    
	LowVK::FreeCommandBuffers(mCopyCommandPool, { cmdBuffer });
}

void Buffer::Map(const uint64_t offset, const uint64_t size, void * data)
{
	/*void* dataPtr;
	vkMapMemory(*mDevicePtr, mMemory, offset, size, 0, &dataPtr);
	memcpy(dataPtr, &data, size);
	vkUnmapMemory(*mDevicePtr, mMemory);*/
}

void Buffer::Release()
{
	LowVK::DestroyBuffer(mBuffer, nullptr);
	LowVK::FreeMemory(mMemory, nullptr);
}
