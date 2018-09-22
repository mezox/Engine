#pragma once

#include <iostream>
#include "RendererBase.h"

namespace Renderer
{
	class RendererTexture
	{
	public:
		virtual ~RendererTexture() = default;
	};

	class RendererBuffer
	{
	public:
		virtual ~RendererBuffer() = default;
	};

	class RendererEffect
	{
	public:
		virtual ~RendererEffect() = default;
	};

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

	class CommandList
	{
	public:
		virtual ~CommandList() = default;
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		template<typename ObjectCreateFunc>
		void CreateGpuObject(const char* objectName, ObjectCreateFunc&& create)
		{
			create();
			std::cout << "Created GPU object: " << objectName << std::endl;
		}

		void OnGpuObjectCreated()
		{

		}
	};
}