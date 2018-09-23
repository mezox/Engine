#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "GraphicsBase.h"

#ifndef VK_NO_PROTOTYPES
#define VK_NO_PROTOTYPES
#endif

#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal : VkResult is \"" << std::endl;												\
	}																									\
}

class LowVK
{
public:
	static GRAPHICS_API void Initialize(const std::string& appName, void* winCreateFunc);
	static GRAPHICS_API void Deinitialize();
	static GRAPHICS_API std::shared_ptr<VkPhysicalDevice> GetPhysical();
	static GRAPHICS_API std::shared_ptr<VkDevice> GetDevice();
	static GRAPHICS_API VkSurfaceKHR GetSurface();

	static GRAPHICS_API VkPhysicalDeviceProperties& GetPhysicalDeviceProperties();
	static GRAPHICS_API std::vector<VkQueueFamilyProperties>& GetPhysicalDeviceQueueFamilyProperties();
	static GRAPHICS_API void GetPhysicalDeviceFormatProperties(VkFormat format, VkFormatProperties* pFormatProperties);
	static GRAPHICS_API void GetBufferMemoryRequirements(const VkBuffer& buffer, VkMemoryRequirements* pMemoryRequirements);
	static GRAPHICS_API void GetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements);
	static GRAPHICS_API void GetPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties* pMemoryProperties);

	// extension VK_KHR_surface
	static GRAPHICS_API void GetPhysicalDeviceSurfaceCapabilitiesKHR(VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
	static GRAPHICS_API void GetPhysicalDeviceSurfaceFormatsKHR(std::vector<VkSurfaceFormatKHR>& surfaceFormats);
	static GRAPHICS_API void GetPhysicalDeviceSurfacePresentModesKHR(std::vector<VkPresentModeKHR>& pPresentModes);
	static GRAPHICS_API void GetPhysicalDeviceSurfaceSupportKHR(uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);

	static GRAPHICS_API void GetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
	static GRAPHICS_API void CreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
	static GRAPHICS_API void DeviceWaitIdle();

	static GRAPHICS_API void CreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
	static GRAPHICS_API void DestroyFramebuffer(VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void CreateImage(const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);
	static GRAPHICS_API void DestroyImage(VkImage image, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void CreateImageView(const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);
	
	
	static GRAPHICS_API void CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
	static GRAPHICS_API void DestroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void AllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
	static GRAPHICS_API void CreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
	static GRAPHICS_API void DestroyDescriptorPool(VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void UpdateDescriptorSets(const std::vector<VkWriteDescriptorSet>& descriptorWrites, const std::vector<VkCopyDescriptorSet>& descriptorCopies);

	static GRAPHICS_API void CreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
	static GRAPHICS_API void CreateGraphicsPipelines(VkPipelineCache pipelineCache, const std::vector<VkGraphicsPipelineCreateInfo>& createInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

	static GRAPHICS_API void CreateSemaphores(const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
	static GRAPHICS_API void CreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
	static GRAPHICS_API void DestroyRenderPass(VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void CreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
	static GRAPHICS_API void DestroyShaderModule(VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void CreateBuffer(const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
	static GRAPHICS_API void DestroyBuffer(VkBuffer buffer, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void CreateSampler(const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler);
	static GRAPHICS_API void DestroySampler(VkSampler sampler, const VkAllocationCallbacks* pAllocator);

	static GRAPHICS_API void MapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
	static GRAPHICS_API void UnmapMemory(VkDeviceMemory memory);
	static GRAPHICS_API void FreeMemory(VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);

	// extension VK_KHR_swapchain
	static GRAPHICS_API void CreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
	static GRAPHICS_API void DestroySwapchainKHR(VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void GetSwapchainImagesKHR(VkSwapchainKHR swapchain, std::vector<VkImage>& images);
	static GRAPHICS_API void AcquireNextImageKHR(VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
	static GRAPHICS_API void QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);

	static GRAPHICS_API void DestroyCommandPool(VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);
	static GRAPHICS_API void DestroyImageView(VkImageView imageView, const VkAllocationCallbacks* pAllocator);

	static GRAPHICS_API void AllocateMemory(const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);
	static GRAPHICS_API void BindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
	static GRAPHICS_API void BindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
	static GRAPHICS_API void CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, const std::vector<VkBufferImageCopy>& regions);
	
	static GRAPHICS_API void CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, const std::vector<VkBufferCopy>& regions);
	static GRAPHICS_API void CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
	static GRAPHICS_API void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, const std::vector<VkDescriptorSet>& descriptorSets, const std::vector<uint32_t>& pDynamicOffsets);
	static GRAPHICS_API void CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
	static GRAPHICS_API void CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
	static GRAPHICS_API void CmdEndRenderPass(VkCommandBuffer commandBuffer);
	static GRAPHICS_API void CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, const std::vector<VkViewport>& viewports);
	static GRAPHICS_API void CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, const std::vector<VkRect2D>& scissors);
	static GRAPHICS_API void CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
	static GRAPHICS_API void AllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
	static GRAPHICS_API void FreeCommandBuffers(VkCommandPool commandPool, const std::vector<VkCommandBuffer>& commandBuffers);
	static GRAPHICS_API void BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);
	static GRAPHICS_API void EndCommandBuffer(VkCommandBuffer commandBuffer);
	static GRAPHICS_API void ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
	static GRAPHICS_API void CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, const std::vector<VkMemoryBarrier>& memoryBarriers, const std::vector<VkBufferMemoryBarrier>& bufferMemoryBarriers, const std::vector<VkImageMemoryBarrier>& imageMemoryBarriers);


	static GRAPHICS_API void QueueWaitIdle(const VkQueue& queue);
	static GRAPHICS_API void QueueSubmit(const VkQueue& queue, const std::vector<VkSubmitInfo>& submits, const VkFence& fence);

	static GRAPHICS_API void CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
	static GRAPHICS_API void CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

private:
	static void SetupDebugCallback();
	static void CreateSurface(void* winCreateFunc);

    static void LoadGlobalFunctions();
	static void LoadInstanceFunctions();
	static void LoadInstanceExtensions();
	static void LoadDeviceFunctions();

private:
    static void* mVulkanLibrary;
    
	static const std::vector<const char*> mEnabledInstanceValidationLayers;
	static const std::vector<const char*> mEnabledInstanceExtensions;
	static const std::vector<const char*> mEnabledDeviceValidationLayers;
	static const std::vector<const char*> mEnabledDeviceExtensions;

	static VkInstance mInstance;
	static VkDebugUtilsMessengerEXT mCallback;
	static VkSurfaceKHR mSurface;
	static std::shared_ptr<VkPhysicalDevice> mPhysicalDevice;
	static std::shared_ptr<VkDevice> mDevice;

	static VkPhysicalDeviceProperties mPhysicalDeviceProperties;
	static std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;
};
