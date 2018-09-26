#include "Effect.h"
#include "RendererResourceStateVK.h"
#include "files.h"
#include "RenderAPIImpl.h"

using namespace Renderer;
using namespace Engine;

void Effect::Build(const std::string& vertex, const std::string& fragment)
{
	const auto vertShaderCode = readFile(vertex);
	const auto fragShaderCode = readFile(fragment);

    const auto renderer = Engine::RenderAPIServiceLocator::Service();
    
	renderer->CreateShader(mVertexShader, vertShaderCode);
	renderer->CreateShader(mFragmentShader, fragShaderCode);
}
