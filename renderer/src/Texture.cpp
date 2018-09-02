#include <Renderer/include/Texture.h>
#include <Renderer/include/RendererVK.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Renderer/include/stb_image.h>

using namespace renderer;

Texture::Texture(const std::shared_ptr<RendererVK>& renderer, const std::string& path)
	: mRenderer(renderer)
{
	int texWidth, texHeight, texChannels;
	mData = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	
	mSize = texWidth * texHeight * 4;
	mWidth = texWidth;
	mHeight = texHeight;

	if (!mData)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	mUsage = ImageUsage::Sampled;
	mFormat = ImageFormat::RGBA8;

	mRenderer->CreateTexture(*this);
}

Texture::Texture(const std::shared_ptr<RendererVK>& renderer, ImageFormat format, ImageUsage usage, uint32_t width, uint32_t height)
	: mRenderer(renderer)
	, mFormat(format)
	, mUsage(usage)
	, mWidth(width)
	, mHeight(height)
{
	mRenderer->CreateTexture(*this);
}

void Texture::SetRendererResource(std::unique_ptr<RendererTexture> resource)
{
	mTexture = std::move(resource); 
}

void renderer::Texture::ClearLocalData()
{
	stbi_image_free(mData);
}
