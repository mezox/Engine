#pragma once

#include <PAL/Graphics/LowVK.h>

#include "Vertex.h"
#include <memory>
#include <cstdint>

#include "Buffer.h"
#include "Renderer.h"

namespace renderer
{
	class Texture;

	uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);

	namespace detail
	{
		VkDeviceQueueCreateInfo CreateQueues(const uint32_t familyIndex, const std::vector<float>& priorities);
	}

    class RendererVK : public Renderer
    {
    public:
        RendererVK() = default;
        
        void Initialize(void* window);
		void Deinitialize();
        
        void CreateBuffer(const BufferDesc& desc, const BufferData& data, std::shared_ptr<Buffer>& bufferObject);
		void CopyBuffer(std::shared_ptr<Buffer>& srcBuffer, std::shared_ptr<Buffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size);

		// void RendererInterface
		void TransitionImageLayout(VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CreateTexture(Texture& texture);

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return mLayout; }
		void CreateDescriptorSetLayout();

		VkShaderModule CreateShader(const std::vector<char>& sourceCode);

		// TODO: delete
		const VkDevice& GetDevice() const { return *mDevice.get(); }
		const VkCommandPool& GetPool() const { return mCmdPool; }
		const VkQueue& GetGraphicsQueue() const { return mGraphicsQueue; }
		const VkSwapchainKHR& GetSwapChain() const { return mSwapChain; }
		const std::vector<VkSurfaceFormatKHR>& GetFormats() const { return mFormats; }
		const std::vector<VkImageView>& GetSwapImageViews() const { return mSwapChainImageViews; }
        
    private:

		void CopyBufferToImage(const VkBuffer& buf, const VkImage& image, const uint32_t width, const uint32_t height);

		VkCommandBuffer		BeginRecording() const;
		void				EndRecording(const VkCommandBuffer& cmdBuff) const;
		VkImageView			CreateImageView(const VkImage& image, VkFormat format, VkImageAspectFlagBits aspect);
		VkSampler			CreateTextureSampler();

		void CreateSwapChain();
        
    private:
        std::shared_ptr<VkDevice> mDevice;
		std::shared_ptr<VkPhysicalDevice> mPhysicalDevice;
        VkCommandPool mCmdPool;
        VkQueue mGraphicsQueue;
		VkDescriptorSetLayout mLayout;

		// Swap chain properties
		VkSurfaceCapabilitiesKHR mCapabilities;
		std::vector<VkSurfaceFormatKHR> mFormats;
		std::vector<VkPresentModeKHR> mPresentationModes;

		VkSwapchainKHR mSwapChain;
		std::vector<VkImage> mSwapChainImages;
		std::vector<VkImageView> mSwapChainImageViews;
    };
}
