#pragma once

#include <memory>
#include <string>

namespace Renderer
{
	class RendererEffect;
	class RendererVK;

	struct PipelineKey
	{

	};

	class Effect
	{
	public:
		Effect() = default;
		Effect(RendererVK* renderer);

		void Build(const std::string& vertex, const std::string& fragment);

	public:
		std::shared_ptr<RendererEffect>	mVertexShader;
		std::shared_ptr<RendererEffect>	mFragmentShader;

	private:
		RendererVK* mRenderer;

	};
}