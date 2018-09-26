#include "Engine.h"

#include <iostream>

#include "RenderAPIImpl.h"
#include "WindowImpl.h"

using namespace Engine;

std::shared_ptr<IEngine> EngineServiceLocator::mService = nullptr;

std::shared_ptr<IEngine> Engine::CreateEngineService()
{
	return std::make_shared<ImmersiveEngine>();
}

ImmersiveEngine::ImmersiveEngine()
{
	std::cout << "Created Engine" << std::endl;
}

std::unique_ptr<IWindow> ImmersiveEngine::CreateWindow()
{
    return std::make_unique<Window>();
}

void ImmersiveEngine::Initialize()
{
    RenderAPIServiceLocator::Provide(CreateVulkanRenderBackEnd());
}

void ImmersiveEngine::StartFrame()
{
	std::cout << "Engine Start Frame" << std::endl;
}

void ImmersiveEngine::Update()
{
}

void ImmersiveEngine::EndFrame()
{
	std::cout << "Engine Finish Frame" << std::endl;
}

void ImmersiveEngine::DeInitialize()
{
    RenderAPIServiceLocator::Provide(nullptr);
}
