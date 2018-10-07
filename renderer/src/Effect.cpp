#include "Effect.h"
#include "RendererResourceStateVK.h"
#include "RenderAPI.h"
#include <PAL/FileSystem/File.h>

using namespace Renderer;
using namespace Engine;

void Effect::Build(const std::string& vertex, const std::string& fragment)
{
    PAL::FileSystem::File vertexShaderFile(vertex);
    const auto vertShaderCode = vertexShaderFile.Read();
    
    PAL::FileSystem::File fragmentShaderFile(fragment);
	const auto fragShaderCode = fragmentShaderFile.Read();

    const auto renderer = Engine::RenderAPIServiceLocator::Service();
    
	renderer->CreateShader(mVertexShader, vertShaderCode);
	renderer->CreateShader(mFragmentShader, fragShaderCode);
}
