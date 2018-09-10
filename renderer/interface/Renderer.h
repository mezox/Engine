#pragma once

#include <iostream>
#include "RendererBase.h"

namespace renderer
{
	class RendererTexture
	{
	public:
		virtual ~RendererTexture() = default;
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

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

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