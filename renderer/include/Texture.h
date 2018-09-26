#pragma once

#include <string>
#include <memory>
#include "BufferTypes.h"

namespace Renderer
{
    class RendererTexture;
    
	class Texture
	{
		//DISALLOW_COPY_AND_ASSIGN(Texture)

	public:
		Texture();
        virtual ~Texture();
        
		Texture(const std::string& path);
		Texture(ImageFormat format, ImageUsage usage, uint32_t width, uint32_t height);

		void SetRendererResource(std::unique_ptr<RendererTexture> resource);
		const unsigned char* Data() const noexcept { return mData; }
		
        const size_t GetSize() const;
        const uint32_t GetWidth() const;
        const uint32_t GetHeight() const;
        const RendererTexture* GetRendererTexture() const;
        const ImageFormat GetFormat() const;
        const ImageUsage GetUsage() const;

		void ClearLocalData();
		void ReleaseFromServer();

	private:
		unsigned char* mData{ nullptr };
		uint32_t mWidth{ 0 };
		uint32_t mHeight{ 0 };
		ImageFormat mFormat;
		ImageUsage mUsage;

		std::unique_ptr<RendererTexture> mTexture;
	};
}
