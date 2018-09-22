#include "Demo.h"
#include <Engine.h>
#include <iostream>
#include <Window.h>

int main()
{
	DemoApplication app;

	Renderer::EngineServiceLocator::Provide(Renderer::CreateEngineService());

    try
	{
        app.initVulkan();

		while (1)
		{
			Renderer::EngineServiceLocator::Service()->StartFrame();

			app.GetWindow()->Update();
			app.draw();
			Renderer::EngineServiceLocator::Service()->EndFrame();
		}

    } catch (const std::exception& e)
	{
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

	app.cleanup();

	Renderer::EngineServiceLocator::Provide(nullptr);

    return EXIT_SUCCESS;
}