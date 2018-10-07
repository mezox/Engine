#include "Texture.h"
#include "RenderAPI.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Renderer;

Texture::Texture()
{}

Texture::~Texture()
{}

Texture::Texture(const std::string& path)
{
	int texWidth, texHeight, texChannels;
	mData = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    
	mWidth = texWidth;
	mHeight = texHeight;

	if (!mData)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	mUsage = ImageUsage::Sampled;
	mFormat = ImageFormat::RGBA8;

    auto renderer = Engine::RenderAPIServiceLocator::Service();
	renderer->CreateTexture(*this);
}

Texture::Texture(ImageFormat format, ImageUsage usage, uint32_t width, uint32_t height)
    : mWidth(width)
	, mHeight(height)
    , mFormat(format)
    , mUsage(usage)
{
    auto renderer = Engine::RenderAPIServiceLocator::Service();
    renderer->CreateTexture(*this);
}

void Texture::SetRendererResource(std::unique_ptr<RendererTexture> resource)
{
	mTexture = std::move(resource); 
}

const RendererTexture* Texture::GetRendererTexture() const
{
    return mTexture.get();
}

const size_t Texture::GetSize() const
{
    return mWidth * mHeight * 4;
}

const uint32_t Texture::GetWidth() const
{
    return mWidth;
}

const uint32_t Texture::GetHeight() const
{
    return mHeight;
}

const ImageFormat Texture::GetFormat() const
{
    return mFormat;
}

const ImageUsage Texture::GetUsage() const
{
    return mUsage;
}

void Texture::ClearLocalData()
{
	stbi_image_free(mData);
}

void Texture::ReleaseFromServer()
{
	mTexture.reset();
}
