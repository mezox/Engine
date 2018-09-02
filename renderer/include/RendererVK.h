#pragma once

#include "Vertex.h"
#include <memory>
#include <cstdint>

#include "Buffer.h"
#include "Renderer.h"

namespace renderer
{    
	enum class ImageFormat
	{
		RGBA8,
		DEPTH
	};

	enum class ImageUsage
	{
		Sampled,
		DepthAttachment
	};

	uint32_t FindMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags properties, const vk::PhysicalDevice& physicalDevice);

	namespace detail
	{
		vk::DeviceQueueCreateInfo CreateQueues(const uint32_t familyIndex, const std::vector<float>& priorities);
	}

    class RendererVK : public Renderer
    {
    public:
        RendererVK() = default;
        
        void Initialize(void* window);
		void Deinitialize();
        
        void CreateBuffer(const BufferDesc& desc, const BufferData& data, std::shared_ptr<IBuffer>& bufferObject);

		// void RendererInterface
		void TransitionImageLayout(vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		void CreateTexture(Texture& texture);

		const vk::DescriptorSetLayout& GetDescriptorSetLayout() const { return mLayout; }
		void CreateDescriptorSetLayout();

		// TODO: delete
		const vk::Device& GetDevice() const { return *mDevice.get(); }
		const vk::CommandPool& GetPool() const { return mCmdPool; }
		const vk::Queue& GetGraphicsQueue() const { return mGraphicsQueue; }
		const vk::SwapchainKHR& GetSwapChain() const { return mSwapChain; }
		const std::vector<vk::SurfaceFormatKHR>& GetFormats() const { return mFormats; }
		const std::vector<vk::ImageView>& GetSwapImageViews() const { return mSwapChainImageViews; }
        
    private:

		void CopyBufferToImage(const vk::Buffer& buf, const vk::Image& image, const uint32_t width, const uint32_t height);

		vk::CommandBuffer		BeginRecording() const;
		void					EndRecording(const vk::CommandBuffer& cmdBuff) const;
		vk::ImageView			CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlagBits aspect);
		vk::Sampler				CreateTextureSampler();

		void QuerySwapChainProperties();
		void CreateSwapChain();
        
    private:
        std::shared_ptr<vk::Device> mDevice;
		std::shared_ptr<vk::PhysicalDevice> mPhysicalDevice;
        vk::CommandPool mCmdPool;
        vk::Queue mGraphicsQueue;
		vk::DescriptorSetLayout mLayout;


		// Swap chain properties
		vk::SurfaceCapabilitiesKHR mCapabilities;
		std::vector<vk::SurfaceFormatKHR> mFormats;
		std::vector<vk::PresentModeKHR> mPresentationModes;

		vk::SwapchainKHR mSwapChain;
		std::vector<vk::Image> mSwapChainImages;
		std::vector<vk::ImageView> mSwapChainImageViews;
    };
}
