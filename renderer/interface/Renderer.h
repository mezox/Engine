#pragma once

#include <iostream>
#include "RendererBase.h"

namespace renderer
{
	class Texture;

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