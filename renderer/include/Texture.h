#pragma once

#include <string>
#include "Renderer.h"		// TODO: Get rid of this, its here only because of formats and usages

namespace Renderer
{
	class IRenderer;

	class Texture
	{
		//DISALLOW_COPY_AND_ASSIGN(Texture)

	public:
		Texture() = default;
		Texture(IRenderer* renderer, const std::string& path);
		Texture(IRenderer* renderer, ImageFormat format, ImageUsage usage, uint32_t width, uint32_t height);

		void SetRendererResource(std::unique_ptr<RendererTexture> resource);
		const unsigned char* Data() const noexcept { return mData; }
		
		const size_t GetSize() const noexcept { return mSize; }
		const uint32_t GetWidth() const noexcept { return mWidth; }
		const uint32_t GetHeight() const noexcept { return mHeight; }
		const RendererTexture* GetRendererTexture() const noexcept { return mTexture.get(); }
		const ImageFormat GetFormat() const { return mFormat; }
		const ImageUsage GetUsage() const { return mUsage; }

		void ClearLocalData();
		void ReleaseFromServer();

	private:
		IRenderer* mRenderer;
		unsigned char* mData{ nullptr };
		size_t mSize{ 0 };

		uint32_t mWidth{ 0 };
		uint32_t mHeight{ 0 };
		ImageFormat mFormat;
		ImageUsage mUsage;

		std::unique_ptr<RendererTexture> mTexture;
	};
}