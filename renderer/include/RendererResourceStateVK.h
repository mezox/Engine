#include <vulkan/vulkan.h>

#include "RenderAPI.h"

namespace Renderer
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

	class EffectVK : public RendererEffect
	{
	public:
		~EffectVK() = default;

	public:
		VkShaderModule module;
	};
}
