#pragma once

#include <iostream>
#include <memory>
#include "RendererBase.h"

namespace Renderer
{


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
