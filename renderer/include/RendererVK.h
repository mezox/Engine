#pragma once

#include <PAL/Graphics/LowVK.h>

#include "Vertex.h"
#include <memory>
#include <cstdint>

#include "Buffer.h"
#include "Renderer.h"
#include "Effect.h"

#include "SwapChainImpl.h" // fwdcl

namespace Renderer
{
	class Texture;

	namespace detail
	{
		VkDeviceQueueCreateInfo CreateQueues(const uint32_t familyIndex, const std::vector<float>& priorities);
	}

	class OneTimeCommandListVK : public CommandList
	{
	public:
		OneTimeCommandListVK(std::shared_ptr<VkCommandPool> cmdPool, std::shared_ptr<VkQueue> queue);
		~OneTimeCommandListVK();

		const VkCommandBuffer GetHandle() const { return mCommandBuffer; }

	private:
		std::shared_ptr<VkCommandPool> mCmdPool;
		std::shared_ptr<VkQueue> mSubmitQueue;
		VkCommandBuffer mCommandBuffer;
	};

    class RendererVK : public IRenderer
    {
    public:
        RendererVK() = default;
        
        void Initialize(void* window);
		void Deinitialize();

		static uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);
        
        void CreateBuffer(const BufferDesc& desc, const BufferData& data, std::shared_ptr<Buffer>& bufferObject);
		void CopyBuffer(std::shared_ptr<Buffer>& srcBuffer, std::shared_ptr<Buffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size);
		void DeleteBuffer(std::shared_ptr<Buffer>& bufferObject);
		void MapMemory(std::shared_ptr<Buffer>& bufferObject);

		// void RendererInterface
		void TransitionImageLayout(VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CreateTexture(Texture& texture);

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return mLayout; }
		void CreateDescriptorSetLayout();

		void CreateShader(std::shared_ptr<RendererEffect>& gpuEffect, const std::vector<char>& sourceCode);
		void CreatePipeline(Effect& effect);
		
		// TODO: delete
		const VkDevice& GetDevice() const { return *mDevice.get(); }
		const VkCommandPool& GetPool() const { return *mCmdPool.get(); }
		const VkQueue& GetGraphicsQueue() const { return *mGraphicsQueue.get(); }
		
		ISwapChain* GetSwapChain();
        
    private:

		void CopyBufferToImage(const VkBuffer& buf, const VkImage& image, const uint32_t width, const uint32_t height);

		VkImageView			CreateImageView(const VkImage& image, VkFormat format, VkImageAspectFlagBits aspect);
		VkSampler			CreateTextureSampler();
        
    private:
        std::shared_ptr<VkDevice> mDevice;
		std::shared_ptr<VkPhysicalDevice> mPhysicalDevice;
        std::shared_ptr<VkCommandPool> mCmdPool;
        std::shared_ptr<VkQueue> mGraphicsQueue;
		VkDescriptorSetLayout mLayout;

		std::unique_ptr<ISwapChain> mSwapChain;
    };
}
