#include "Effect.h"
#include "RendererResourceStateVK.h"
#include "files.h"
#include "RendererVK.h"

using namespace Renderer;

Effect::Effect(RendererVK* renderer)
	: mRenderer(renderer)
{
}

void Effect::Build(const std::string& vertex, const std::string& fragment)
{
	const auto vertShaderCode = readFile(vertex);
	const auto fragShaderCode = readFile(fragment);

	mRenderer->CreateShader(mVertexShader, vertShaderCode);
	mRenderer->CreateShader(mFragmentShader, fragShaderCode);
}
