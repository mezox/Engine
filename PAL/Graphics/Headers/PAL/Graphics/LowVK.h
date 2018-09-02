#pragma once

#include <string>
#include <vector>

#include "GraphicsBase.h"
#include <vulkan/vulkan.hpp>

class LowVK
{
public:
	static GRAPHICS_API void Initialize(const std::string& appName, void* winCreateFunc);
	static GRAPHICS_API void Deinitialize();
	static GRAPHICS_API std::shared_ptr<vk::PhysicalDevice> GetPhysical();
	static GRAPHICS_API std::shared_ptr<vk::Device> GetDevice();
	static GRAPHICS_API vk::SurfaceKHR GetSurface();

private:
	static void SetupDebugCallback();
	static void CreateSurface(void* winCreateFunc);

private:
	static const std::vector<const char*> mEnabledInstanceValidationLayers;
	static const std::vector<const char*> mEnabledInstanceExtensions;
	static const std::vector<const char*> mEnabledDeviceValidationLayers;
	static const std::vector<const char*> mEnabledDeviceExtensions;

	static vk::Instance mInstance;
	static vk::DebugUtilsMessengerEXT mCallback;
	static vk::DispatchLoaderDynamic mDldy;
	static vk::SurfaceKHR mSurface;
	static std::shared_ptr<vk::PhysicalDevice> mPhysicalDevice;
	static std::shared_ptr<vk::Device> mDevice;
};