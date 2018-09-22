#include "Engine.h"

#include <iostream>

using namespace Renderer;

std::shared_ptr<IEngine> EngineServiceLocator::mService = nullptr;

std::shared_ptr<IEngine> Renderer::CreateEngineService()
{
	return std::make_shared<Engine>();
}

Engine::Engine()
{
	std::cout << "Created Engine" << std::endl;
}

void Engine::StartFrame()
{
	std::cout << "Engine Start Frame" << std::endl;
}

void Engine::Update()
{
}

void Engine::EndFrame()
{
	std::cout << "Engine Finish Frame" << std::endl;
}
