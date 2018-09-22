#include "RendererResourceStateVK.h"

#include <PAL/Graphics/LowVK.h>

using namespace Renderer;


RendererTextureVK::~RendererTextureVK()
{
	LowVK::DestroyImage(image, nullptr);
	LowVK::DestroyImageView(imageView, nullptr);

	if (sampler != VK_NULL_HANDLE)
	{
		LowVK::DestroySampler(sampler, nullptr);
	}

	LowVK::FreeMemory(memory, nullptr);
}

BufferVK::~BufferVK()
{
	LowVK::DestroyBuffer(buffer, nullptr);
	LowVK::FreeMemory(memory, nullptr);
}
