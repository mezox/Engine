#include "Engine.h"

#include <iostream>

#include "RenderAPIImpl.h"
#include "WindowImpl.h"
#include <PAL/FileSystem/LowFileSystem.h>

using namespace Engine;
using namespace PAL::FileSystem;

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
    FileSystemServiceLocator::Provide(CreateFileSystemService());
    RenderAPIServiceLocator::Provide(CreateVulkanRenderBackEnd());
    
    FileSystemServiceLocator::Service()->Initialize();
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
    FileSystemServiceLocator::Service()->DeInitialize();
    
    RenderAPIServiceLocator::Provide(nullptr);
    FileSystemServiceLocator::Provide(nullptr);
}
