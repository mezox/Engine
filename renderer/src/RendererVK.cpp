#include "RendererVK.h"

#include "BufferImpl.h"
#include "Texture.h"
#include <PAL/Graphics/Headers/PAL/Graphics/LowVK.h>

//#include <vulkan/vulkan.hpp>

#ifdef WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>

#undef min
#undef max
#endif

using namespace renderer;

namespace
{
	vk::Format ConvertImageFormat(const ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGBA8:
			return vk::Format::eR8G8B8A8Unorm;
		case ImageFormat::DEPTH:
			return vk::Format::eD24UnormS8Uint;
		default:
			break;
		}

		return vk::Format::eR8G8B8A8Unorm;
	}

	vk::ImageUsageFlagBits ConvertImageUsage(const ImageUsage usage)
	{
		switch (usage)
		{
		case ImageUsage::Sampled: return vk::ImageUsageFlagBits::eSampled;
		case ImageUsage::DepthAttachment: return vk::ImageUsageFlagBits::eDepthStencilAttachment;

		default:
			break;
		}

		return vk::ImageUsageFlagBits::eSampled;
	}
}

namespace {
	uint32_t width = 1600;
	uint32_t height = 990;
}

vk::DeviceQueueCreateInfo detail::CreateQueues(const uint32_t familyIndex, const std::vector<float>& priorities)
{
	vk::DeviceQueueCreateInfo createInfo;
	createInfo.queueCount = static_cast<uint32_t>(priorities.size());
	createInfo.queueFamilyIndex = familyIndex;
	createInfo.pQueuePriorities = priorities.data();

	return createInfo;
}

void RendererVK::QuerySwapChainProperties()
{
	const auto surface = LowVK::GetSurface();

	mCapabilities = mPhysicalDevice->getSurfaceCapabilitiesKHR(surface);
	mFormats = mPhysicalDevice->getSurfaceFormatsKHR(surface);
	mPresentationModes = mPhysicalDevice->getSurfacePresentModesKHR(surface);
}

void RendererVK::CreateSwapChain()
{
	if (mPhysicalDevice->getSurfaceSupportKHR(0, LowVK::GetSurface()))
	{
		const auto& format = mFormats.front();      // Get B8G8R8A8_unorm
		const auto& presentMode = mPresentationModes.front();   // Immeadiate
		vk::Extent2D screenExtent(::width, ::height);

		uint32_t imageCount = mCapabilities.minImageCount + 1;

		if (1/*mEnableTrippleBuffering*/)
		{
			if (mCapabilities.maxImageCount > 0 && imageCount > mCapabilities.maxImageCount)
			{
				imageCount = mCapabilities.maxImageCount;
			}
		}

		vk::SwapchainCreateInfoKHR createInfo;
		createInfo.surface = LowVK::GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.setImageColorSpace(format.colorSpace);
		createInfo.setImageFormat(format.format);
		createInfo.setImageExtent(screenExtent);
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;       //Use transfer if I want to render to other buffer and apply post process and then copy;
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
		createInfo.preTransform = mCapabilities.currentTransform;
		createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = vk::SwapchainKHR{};

		try {

			mSwapChain = mDevice->createSwapchainKHR(createInfo);
		}
		catch (std::exception e)
		{
			std::cout << "Failed to create swap chain: " << e.what() << std::endl;
		}

		mSwapChainImages = mDevice->getSwapchainImagesKHR(mSwapChain);

		mSwapChainImageViews.resize(mSwapChainImages.size());
		for (size_t idx{ 0 }; idx < mSwapChainImageViews.size(); ++idx)
		{
			vk::ImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.image = mSwapChainImages[idx];
			imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
			imageViewCreateInfo.format = format.format;
			imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			mSwapChainImageViews[idx] = mDevice->createImageView(imageViewCreateInfo);
		}
	}
}


void RendererVK::Initialize(void* window)
{
	LowVK::Initialize("Engine", glfwGetWin32Window((GLFWwindow*)window));

	mPhysicalDevice = LowVK::GetPhysical();
	mDevice = LowVK::GetDevice();
	const auto queueProperties = mPhysicalDevice->getQueueFamilyProperties();

	std::cout << "Available queue families: " << queueProperties.size() << std::endl;
	for (const auto& queueFamily : queueProperties)
	{
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			std::cout << "G | ";
		if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
			std::cout << "C | ";
		if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer)
			std::cout << "T | ";
		if (queueFamily.queueFlags & vk::QueueFlagBits::eProtected)
			std::cout << "P | ";
		if (queueFamily.queueFlags & vk::QueueFlagBits::eSparseBinding)
			std::cout << "S | ";

		std::cout << "Queue count: " << queueFamily.queueCount << std::endl;
	}

	mGraphicsQueue = mDevice->getQueue(0, 0);

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.queueFamilyIndex = 0;

	try
	{
		mCmdPool = mDevice->createCommandPool(poolInfo);
	}
	catch (std::exception e)
	{
		std::cout << "Failed to create command pool: " << e.what() << std::endl;
	}

	QuerySwapChainProperties();
	CreateSwapChain();
}

void RendererVK::Deinitialize()
{
	mDevice->waitIdle();

	for (auto& imageView : mSwapChainImageViews)
	{
		mDevice->destroyImageView(imageView);
	}

	mDevice->destroySwapchainKHR(mSwapChain);
	LowVK::Deinitialize();
}

void RendererVK::CreateBuffer(const BufferDesc& desc, const BufferData& data, std::shared_ptr<IBuffer>& bufferObject)
{
    bufferObject = std::make_shared<renderer::Buffer>(mDevice, *mPhysicalDevice, mCmdPool, mGraphicsQueue, desc, data);
}

void RendererVK::TransitionImageLayout(vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
	auto cmdBuffer = BeginRecording();

	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
	}
	else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
	{
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
		barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	cmdBuffer.pipelineBarrier(sourceStage, destinationStage, {}, {}, {}, { barrier });

	EndRecording(cmdBuffer);
};

void RendererVK::CreateTexture(Texture& texture)
{
	const bool shouldUploadData = texture.Data() != nullptr;

	const auto usage = texture.GetUsage();
	const auto format = texture.GetFormat();

	std::unique_ptr<Buffer> stagingBuffer;

	if(shouldUploadData)
	{
		BufferDesc bufDesc;
		bufDesc.usage = BufferUsage::Staging | BufferUsage::TransferSrc;

		BufferData texData;
		texData.data = (void*)texture.Data();
		texData.size = static_cast<uint32_t>(texture.GetSize());

		stagingBuffer = std::make_unique<Buffer>(mDevice, *mPhysicalDevice, mCmdPool, mGraphicsQueue, bufDesc, texData);

		const auto& stagingMemory = stagingBuffer->GetVulkanMemory();

		void* data = mDevice->mapMemory(stagingMemory, 0, texData.size);
		memcpy(data, texture.Data(), (size_t)texData.size);
		mDevice->unmapMemory(stagingMemory);

		texture.ClearLocalData();
	}

	auto tex = std::make_unique<RendererTextureVK>();

	vk::ImageCreateInfo imageInfo;
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = texture.GetWidth();
	imageInfo.extent.height = texture.GetHeight();
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = ConvertImageFormat(format);
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;
	imageInfo.samples = vk::SampleCountFlagBits::e1;
	imageInfo.usage = ConvertImageUsage(usage);

	if (usage == ImageUsage::DepthAttachment)
	{
		const std::vector<vk::Format> candidates{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint };

		for (vk::Format form : candidates)
		{
			vk::FormatProperties props = mPhysicalDevice->getFormatProperties(form);

			if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) == vk::FormatFeatureFlagBits::eDepthStencilAttachment)
			{
				break;
			}
		}
	}

	if (shouldUploadData)
	{
		imageInfo.usage |= vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	}

	try
	{
		tex->image = mDevice->createImage(imageInfo);
	}
	catch (std::exception e)
	{
		std::cout << "Create texture: " << e.what();
	}

	vk::MemoryRequirements memoryRequirements = mDevice->getImageMemoryRequirements(tex->image);
	vk::MemoryPropertyFlags memoryFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;

	vk::MemoryAllocateInfo memAllocInfo;
	memAllocInfo.allocationSize = memoryRequirements.size;
	memAllocInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, memoryFlags, *mPhysicalDevice.get());
	tex->memory = mDevice->allocateMemory(memAllocInfo);

	mDevice->bindImageMemory(tex->image, tex->memory, 0);

	if (shouldUploadData)
	{
		TransitionImageLayout(tex->image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
		CopyBufferToImage(stagingBuffer->GetVulkanBuffer(), tex->image, texture.GetWidth(), texture.GetHeight());
		TransitionImageLayout(tex->image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
	}
	else if (usage == ImageUsage::DepthAttachment)
	{
		TransitionImageLayout(tex->image, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);
	}

	if (usage == ImageUsage::Sampled)
	{
		tex->sampler = CreateTextureSampler();
	}

	tex->imageView = CreateImageView(tex->image, ConvertImageFormat(texture.GetFormat()), (usage != ImageUsage::DepthAttachment) ? vk::ImageAspectFlagBits::eColor : vk::ImageAspectFlagBits::eDepth);

	texture.SetRendererResource(std::move(tex));
}

void RendererVK::CopyBufferToImage(const vk::Buffer& buf, const vk::Image & image, const uint32_t width, const uint32_t height)
{
	auto cmdBuffer = BeginRecording();

	vk::BufferImageCopy region;
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	try
	{
		cmdBuffer.copyBufferToImage(buf, image, vk::ImageLayout::eTransferDstOptimal, { region });
	}
	catch (std::exception e)
	{
		std::cout << "RendererVK::CopyBufferToImage: " << e.what();
	}
	
	EndRecording(cmdBuffer);
}

vk::CommandBuffer renderer::RendererVK::BeginRecording() const
{
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandPool = mCmdPool;
	allocInfo.commandBufferCount = 1;

	auto commandBuffers = mDevice->allocateCommandBuffers(allocInfo);

	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

	auto& cmdBuffer = commandBuffers.back();
	cmdBuffer.begin(beginInfo);

	return cmdBuffer;
}

void RendererVK::EndRecording(const vk::CommandBuffer& cmdBuff) const
{
	cmdBuff.end();

	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuff;

	mGraphicsQueue.submit(1, &submitInfo, {});
	mGraphicsQueue.waitIdle();

	// We could use a fence and wait with vkWaitForFences, or simply wait for the transfer queue to become idle with vkQueueWaitIdle. A fence would allow you to schedule multiple
	// transfers simultaneously and wait for all of them complete, instead of executing one at a time. That may give the driver more opportunities to optimize.

	mDevice->freeCommandBuffers(mCmdPool, 1, &cmdBuff);
	// ----------------------------------------------------------
}

vk::ImageView RendererVK::CreateImageView(vk::Image image, vk::Format format, vk::ImageAspectFlagBits aspect)
{
	vk::ImageViewCreateInfo viewInfo;
	viewInfo.image = image;
	viewInfo.viewType = vk::ImageViewType::e2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	vk::ImageView view;

	try
	{
		view = mDevice->createImageView(viewInfo);
	}
	catch (std::exception e)
	{
		std::cout << "Failed to create image view" << std::endl;
	}

	return view;
}

vk::Sampler RendererVK::CreateTextureSampler()
{
	vk::SamplerCreateInfo samplerInfo;
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.minFilter = vk::Filter::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 1;
	samplerInfo.borderColor = vk::BorderColor::eFloatOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = vk::CompareOp::eAlways;
	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;

	vk::Sampler sampler;

	try
	{
		sampler = mDevice->createSampler(samplerInfo);
	}
	catch (std::exception e)
	{
		std::cout << "Failed to create sampler: " << e.what() << std::endl;
	}

	return sampler;
}

void RendererVK::CreateDescriptorSetLayout()
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding;
	uboLayoutBinding.binding = 0;											// Binding references in shader.
	uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
	uboLayoutBinding.descriptorCount = 1;									// If > 1 represents array in shader.
	uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;			// Shader stage to use.
	uboLayoutBinding.pImmutableSamplers = nullptr;							// For image sampling related descriptors.

	vk::DescriptorSetLayoutBinding samplerLayoutBinding;
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

	vk::DescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	try
	{
		mLayout = mDevice->createDescriptorSetLayout(layoutInfo);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

uint32_t renderer::FindMemoryType(uint32_t typeFilter, const vk::MemoryPropertyFlags properties, const vk::PhysicalDevice & physicalDevice)
{
	vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}
