#include "RendererVK.h"

#include "Texture.h"
#include "RendererResourceStateVK.h"
#include "SwapChainImpl.h"

#ifdef WIN32
#include <vulkan/vulkan_win32.h>

#undef min
#undef max
#endif

using namespace Renderer;

namespace
{
	VkFormat ConvertImageFormat(const ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RGBA8:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case ImageFormat::DEPTH:
			return VK_FORMAT_D24_UNORM_S8_UINT;
		default:
			break;
		}

		return VK_FORMAT_R8G8B8A8_UNORM;
	}

	VkImageUsageFlagBits ConvertImageUsage(const ImageUsage usage)
	{
		switch (usage)
		{
		case ImageUsage::Sampled: return VK_IMAGE_USAGE_SAMPLED_BIT;
		case ImageUsage::DepthAttachment: return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

		default:
			break;
		}

		return VK_IMAGE_USAGE_SAMPLED_BIT;
	}
}

namespace {
	uint32_t width = 1920;
	uint32_t height = 1080;
}

VkDeviceQueueCreateInfo detail::CreateQueues(const uint32_t familyIndex, const std::vector<float>& priorities)
{
	VkDeviceQueueCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	createInfo.queueCount = static_cast<uint32_t>(priorities.size());
	createInfo.queueFamilyIndex = familyIndex;
	createInfo.pQueuePriorities = priorities.data();

	return createInfo;
}

void RendererVK::Initialize(void* window)
{
	LowVK::Initialize("Engine", window);

	mDevice = LowVK::GetDevice();
	mPhysicalDevice = LowVK::GetPhysical();

	const auto& queueProperties = LowVK::GetPhysicalDeviceQueueFamilyProperties();

	std::cout << "Available queue families: " << queueProperties.size() << std::endl;
	for (const auto& queueFamily : queueProperties)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			std::cout << "G | ";
		if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
			std::cout << "C | ";
		if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
			std::cout << "T | ";
		if (queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT)
			std::cout << "P | ";
		if (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
			std::cout << "S | ";

		std::cout << "Queue count: " << queueFamily.queueCount << std::endl;
	}

	mGraphicsQueue = std::make_shared<VkQueue>();
	LowVK::GetDeviceQueue(0, 0, mGraphicsQueue.get());


	mCmdPool = std::make_shared<VkCommandPool>();
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = 0;

	LowVK::CreateCommandPool(&poolInfo, nullptr, mCmdPool.get());

	mSwapChain = std::make_unique<SwapChainVK>();
	mSwapChain->Initialize(this, ::width, ::height);
}

void RendererVK::Deinitialize()
{
	LowVK::DeviceWaitIdle();

	LowVK::DestroyCommandPool(*mCmdPool.get(), nullptr);
	LowVK::DestroyDescriptorSetLayout(mLayout, nullptr);
	LowVK::Deinitialize();
}

void RendererVK::CreateBuffer(const BufferDesc& desc, const BufferData& data, std::shared_ptr<Buffer>& bufferObject)
{
	bufferObject = std::make_shared<Buffer>();

	auto bufferVk = std::make_unique<BufferVK>();

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = data.size;

	if (desc.flags & BufferBindFlags::VertexBuffer)
		bufferInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	if (desc.flags & BufferBindFlags::IndexBuffer)
		bufferInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	if (desc.flags & BufferBindFlags::UniformBuffer)
		bufferInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	if (desc.usage & BufferUsage::TransferDest)
		bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	if (desc.usage & BufferUsage::TransferSrc)
		bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	LowVK::CreateBuffer(&bufferInfo, nullptr, &bufferVk->buffer);

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
	LowVK::GetBufferMemoryRequirements(bufferVk->buffer, &memoryRequirements);

	VkMemoryPropertyFlags memoryFlags;
	if (desc.usage & BufferUsage::Staging)
		memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	else
		memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryFlags);

	LowVK::AllocateMemory(&allocInfo, nullptr, &bufferVk->memory);
	LowVK::BindBufferMemory(bufferVk->buffer, bufferVk->memory, 0);

	bufferObject->SetGpuResource(std::move(bufferVk));
}

void RendererVK::CopyBuffer(std::shared_ptr<Buffer>& srcBuffer, std::shared_ptr<Buffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size)
{
	OneTimeCommandListVK cmdBuf(mCmdPool, mGraphicsQueue);

	VkBufferCopy copyInfo;
	copyInfo.srcOffset = srcOffset; // Optional
	copyInfo.dstOffset = dstOffset; // Optional
	copyInfo.size = size;

	auto srcBufferVk = (BufferVK*)(srcBuffer->GetGpuResource());
	auto dstBufferVk = (BufferVK*)(dstBuffer->GetGpuResource());

	LowVK::CmdCopyBuffer(cmdBuf.GetHandle(), srcBufferVk->buffer, dstBufferVk->buffer, { copyInfo });
}

void RendererVK::DeleteBuffer(std::shared_ptr<Buffer>& bufferObject)
{
	auto gpuBuffer = (BufferVK*)(bufferObject->GetGpuResource());
	LowVK::DestroyBuffer(gpuBuffer->buffer, nullptr);
	LowVK::FreeMemory(gpuBuffer->memory, nullptr);
}

void RendererVK::MapMemory(std::shared_ptr<Buffer>& bufferObject)
{
	//void* data{ nullptr };

	//auto gpuBuffer = (const BufferVK*)(bufferObject->GetGpuResource());

	//LowVK::MapMemory(gpuBuffer->memory, 0, vertexData.size, 0, &data);
	//memcpy(dataPtr, cube.vertices.data(), (size_t)vertexData.size);
	//LowVK::UnmapMemory(gpuBuffer->memory);
}

void RendererVK::TransitionImageLayout(VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	OneTimeCommandListVK cmdBuffer(mCmdPool, mGraphicsQueue);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	LowVK::CmdPipelineBarrier(cmdBuffer.GetHandle(), sourceStage, destinationStage, 0, {}, {}, { barrier });
};

void RendererVK::CreateTexture(Texture& texture)
{
	const bool shouldUploadData = texture.Data() != nullptr;

	const auto usage = texture.GetUsage();
	const auto format = texture.GetFormat();

	std::shared_ptr<Buffer> stagingBuffer;

	if(shouldUploadData)
	{
		BufferDesc bufDesc;
		bufDesc.usage = BufferUsage::Staging | BufferUsage::TransferSrc;

		BufferData texData;
		texData.data = (void*)texture.Data();
		texData.size = static_cast<uint32_t>(texture.GetSize());

		stagingBuffer = std::make_shared<Buffer>();
		CreateBuffer(bufDesc, texData, stagingBuffer);

		auto bufferVk = (BufferVK*)(stagingBuffer->GetGpuResource());
		const auto& stagingMemory = bufferVk->memory;

		void* data{ nullptr };
		LowVK::MapMemory(stagingMemory, 0, texData.size, 0, &data);
		memcpy(data, texture.Data(), (size_t)texData.size);
		LowVK::UnmapMemory(stagingMemory);

		texture.ClearLocalData();
	}

	auto tex = std::make_unique<RendererTextureVK>();

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = texture.GetWidth();
	imageInfo.extent.height = texture.GetHeight();
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = ConvertImageFormat(format);
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.usage = ConvertImageUsage(usage);

	if (usage == ImageUsage::DepthAttachment)
	{
		const std::vector<VkFormat> candidates{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

		for (VkFormat form : candidates)
		{
			VkFormatProperties props;
			LowVK::GetPhysicalDeviceFormatProperties(form, &props);

			if ((props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				break;
			}
		}
	}

	if (shouldUploadData)
	{
		imageInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	}


	LowVK::CreateImage(&imageInfo, nullptr, &tex->image);

	VkMemoryRequirements memoryRequirements;
	LowVK::GetImageMemoryRequirements(tex->image, &memoryRequirements);

	VkMemoryPropertyFlags memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkMemoryAllocateInfo memAllocInfo{};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.allocationSize = memoryRequirements.size;
	memAllocInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, memoryFlags, *mPhysicalDevice.get());
	
	LowVK::AllocateMemory(&memAllocInfo, nullptr, &tex->memory);
	LowVK::BindImageMemory(tex->image, tex->memory, 0);

	if (shouldUploadData)
	{
		auto bufferVk = (BufferVK*)(stagingBuffer->GetGpuResource());

		TransitionImageLayout(tex->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(bufferVk->buffer, tex->image, texture.GetWidth(), texture.GetHeight());
		TransitionImageLayout(tex->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
	else if (usage == ImageUsage::DepthAttachment)
	{
		TransitionImageLayout(tex->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	if (usage == ImageUsage::Sampled)
	{
		tex->sampler = CreateTextureSampler();
	}

	tex->imageView = CreateImageView(tex->image, ConvertImageFormat(texture.GetFormat()), (usage != ImageUsage::DepthAttachment) ? VK_IMAGE_ASPECT_COLOR_BIT : VK_IMAGE_ASPECT_DEPTH_BIT);

	texture.SetRendererResource(std::move(tex));

	/*if (stagingBuffer)
	{
		stagingBuffer->Release();
	}*/
}

ISwapChain* RendererVK::GetSwapChain()
{
	return mSwapChain.get();
}

void RendererVK::CopyBufferToImage(const VkBuffer& buf, const VkImage & image, const uint32_t width, const uint32_t height)
{
	OneTimeCommandListVK cmdBuffer(mCmdPool, mGraphicsQueue);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	LowVK::CmdCopyBufferToImage(cmdBuffer.GetHandle(), buf, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, { region });
}

VkImageView RendererVK::CreateImageView(const VkImage& image, VkFormat format, VkImageAspectFlagBits aspect)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView view;
	LowVK::CreateImageView(&viewInfo, nullptr, &view);

	return view;
}

VkSampler RendererVK::CreateTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 1;
	samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	VkSampler sampler;
	LowVK::CreateSampler(&samplerInfo, nullptr, &sampler);

	return sampler;
}

void RendererVK::CreateDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding;
	uboLayoutBinding.binding = 0;											// Binding references in shader.
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;									// If > 1 represents array in shader.
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;				// Shader stage to use.
	uboLayoutBinding.pImmutableSamplers = nullptr;							// For image sampling related descriptors.

	VkDescriptorSetLayoutBinding samplerLayoutBinding;
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings { uboLayoutBinding, samplerLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	LowVK::CreateDescriptorSetLayout(&layoutInfo, nullptr, &mLayout);
}

void RendererVK::CreateShader(std::shared_ptr<RendererEffect>& gpuEffect, const std::vector<char>& sourceCode)
{
	auto vulkanEffect = std::make_shared<EffectVK>();

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = sourceCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());

	LowVK::CreateShaderModule(&createInfo, nullptr, &vulkanEffect->module);

	gpuEffect = std::move(vulkanEffect);
}

void RendererVK::CreatePipeline(Effect& effect)
{
	auto gpuVertexShader = (EffectVK*)effect.mVertexShader.get();
	auto gpuFragmentShader = (EffectVK*)effect.mVertexShader.get();

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = gpuVertexShader->module;
	vertShaderStageInfo.pName = "main";
	vertShaderStageInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = gpuFragmentShader->module;
	fragShaderStageInfo.pName = "main";

}

OneTimeCommandListVK::OneTimeCommandListVK(std::shared_ptr<VkCommandPool> cmdPool, std::shared_ptr<VkQueue> queue)
	: mCmdPool(std::move(cmdPool))
	, mSubmitQueue(std::move(queue))
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = *mCmdPool.get();
	allocInfo.commandBufferCount = 1;

	LowVK::AllocateCommandBuffers(&allocInfo, &mCommandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	LowVK::BeginCommandBuffer(mCommandBuffer, &beginInfo);
}

OneTimeCommandListVK::~OneTimeCommandListVK()
{
	LowVK::EndCommandBuffer(mCommandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &mCommandBuffer;

	LowVK::QueueSubmit(*mSubmitQueue.get(), { submitInfo }, {});
	LowVK::QueueWaitIdle(*mSubmitQueue.get());

	// We could use a fence and wait with vkWaitForFences, or simply wait for the transfer queue to become idle with vkQueueWaitIdle. A fence would allow you to schedule multiple
	// transfers simultaneously and wait for all of them complete, instead of executing one at a time. That may give the driver more opportunities to optimize.

	LowVK::FreeCommandBuffers(*mCmdPool.get(), { mCommandBuffer });
}

uint32_t RendererVK::FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice & physicalDevice)
{
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

	return 0;
}
