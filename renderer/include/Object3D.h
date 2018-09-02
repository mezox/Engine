#pragma once

#include <vector>

#include "Vertex.h"

namespace Renderer
{
	class Object3D
	{
	public:
		Object3D() = default;

	public:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class Cube : public Object3D
	{
	public:
		Cube();
	};

	class Mesh : public Object3D
	{
	public:
		void Load(const char* path);
		
	public:

	};
}