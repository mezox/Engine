#include "Demo.h"
#include <iostream>

int main() {
	DemoApplication app;

    try
	{
        app.initVulkan();

		while (!glfwWindowShouldClose(app.window))
		{
			glfwPollEvents();
			app.draw();
		}

    } catch (const std::exception& e)
	{
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

	glfwDestroyWindow(app.window);
	glfwTerminate();

    return EXIT_SUCCESS;
}