#pragma once

#include <PAL/Graphics/LowVK.h>

#ifdef WIN32
#	ifndef VK_USE_PLATFORM_WIN32_KHR
#		define VK_USE_PLATFORM_WIN32_KHR
#	endif

#include <vulkan/vulkan_win32.h>

const std::vector<const char*> LowVK::mEnabledInstanceExtensions =
{
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
};
#else
#include <vulkan/vulkan_macos.h>
const std::vector<const char*> extensions =
{
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_MACOS_SURFACE_EXTENSION_NAME,
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};
#endif

const std::vector<const char*> LowVK::mEnabledInstanceValidationLayers
{
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> LowVK::mEnabledDeviceValidationLayers;
const std::vector<const char*> LowVK::mEnabledDeviceExtensions
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};