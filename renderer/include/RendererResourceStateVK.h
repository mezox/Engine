#include <vulkan/vulkan.h>

#include "Renderer.h"

namespace renderer
{
	class RendererTextureVK : public RendererTexture
	{
	public:
		~RendererTextureVK();

	public:
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory memory;
		VkSampler sampler;
	};

	class BufferVK : public RendererBuffer
	{
	public:
		~BufferVK();

	public:
		VkBuffer buffer;
		VkDeviceMemory memory;
	};
}