#include <PAL/Graphics/LowVK.h>
#include "LayersExtensions.inl"

#include <iostream>

vk::Instance LowVK::mInstance;
vk::DebugUtilsMessengerEXT LowVK::mCallback;
vk::DispatchLoaderDynamic LowVK::mDldy;
vk::SurfaceKHR LowVK::mSurface;
std::shared_ptr<vk::PhysicalDevice> LowVK::mPhysicalDevice;
std::shared_ptr<vk::Device> LowVK::mDevice;

#define CHECK_EX(stmt, msg) try { stmt; } catch (std::exception e) { std::cout << msg << ": " << e.what() << std::endl; }

namespace
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::string msgOut = "[VK ";

		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			msgOut += "Verbose ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			msgOut += "Info ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			msgOut += "Warning ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			msgOut += "Error ";
			break;
		default:
			break;
		}

		switch (messageType)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			msgOut += "General";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			msgOut += "Performance";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			msgOut += "Validation";
			break;
		default:
			break;
		}

		std::cout << msgOut << "] " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void QueryInstanceExtensions()
	{
		auto extensions = vk::enumerateInstanceExtensionProperties();

		std::cout << extensions.size() << " instance extensions available: " << std::endl;

		for (size_t idx = 0; idx < extensions.size(); ++idx)
		{
			std::cout << extensions[idx].extensionName << "\n";
		}
	}

	void QueryInstanceLayers()
	{
		auto layers = vk::enumerateInstanceLayerProperties();

		std::cout << layers.size() << " instance layers available: " << std::endl;

		for (const auto& layer : layers)
		{
			std::cout << layer.layerName << "\n";
		}
	}

	void QueryDeviceExtensions(const vk::PhysicalDevice& device)
	{
		std::cout << "Device extensions: " << std::endl;
		const auto deviceExtensions = device.enumerateDeviceExtensionProperties();
		for (const auto& ext : deviceExtensions)
		{
			std::cout << ext.extensionName << ", v: " << VK_VERSION_MAJOR(ext.specVersion) << "."
				<< VK_VERSION_MAJOR(ext.specVersion) << "."
				<< VK_VERSION_PATCH(ext.specVersion) << std::endl;
		}
	}
}

void LowVK::Initialize(const std::string& appName, void* winCreateFunc)
{
	QueryInstanceLayers();
	QueryInstanceExtensions();

	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(mEnabledInstanceExtensions.size());
	instanceInfo.ppEnabledExtensionNames = mEnabledInstanceExtensions.data();
	instanceInfo.enabledLayerCount = static_cast<uint32_t>(mEnabledInstanceValidationLayers.size());
	instanceInfo.ppEnabledLayerNames = mEnabledInstanceValidationLayers.data();

	CHECK_EX(mInstance = vk::createInstance(instanceInfo), "Failed to create instance");
	CHECK_EX(mDldy.init(mInstance), "Failed to create dynamic loader");

	if (!mEnabledInstanceValidationLayers.empty())
	{
		SetupDebugCallback();
	}

	const auto devices = mInstance.enumeratePhysicalDevices();
	std::cout << "Available GPU devices: " << devices.size() << std::endl;

	for (const auto& device : devices)
	{
		const auto deviceProperties = device.getProperties();

		std::cout << deviceProperties.deviceName
			<< ", Vulkan API version: "
			<< VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
			<< VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
			<< VK_VERSION_PATCH(deviceProperties.apiVersion) << std::endl;
	}

	mPhysicalDevice = std::make_shared<vk::PhysicalDevice>(devices.back());

	QueryDeviceExtensions(*mPhysicalDevice);

	CreateSurface(winCreateFunc);

	const auto queueProperties = mPhysicalDevice->getQueueFamilyProperties(); // this has to be called before creating queues

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

	std::vector<float> queuePriorities{ 1.0f };

	vk::DeviceQueueCreateInfo queueInfo;
	queueInfo.queueCount = static_cast<uint32_t>(queuePriorities.size());
	queueInfo.queueFamilyIndex = 0;
	queueInfo.pQueuePriorities = queuePriorities.data();

	queueCreateInfos.push_back(queueInfo);

	const auto deviceFeatures = mPhysicalDevice->getFeatures();

	vk::DeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(mEnabledDeviceExtensions.size());;
	deviceCreateInfo.ppEnabledExtensionNames = mEnabledDeviceExtensions.data();
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mEnabledDeviceValidationLayers.size());
	deviceCreateInfo.ppEnabledLayerNames = mEnabledDeviceValidationLayers.data();

	CHECK_EX(mDevice = std::make_shared<vk::Device>(mPhysicalDevice->createDevice(deviceCreateInfo)), "Failed to create logical device");
}

void LowVK::Deinitialize()
{
	CHECK_EX(mDevice->destroy(), "Failed to destroy device");
	CHECK_EX(mInstance.destroySurfaceKHR(mSurface), "Failed to destroy surface");
	CHECK_EX(mInstance.destroyDebugUtilsMessengerEXT(mCallback, nullptr, mDldy), "Failed to destroy debug utils messenger");
	CHECK_EX(mInstance.destroy(), "Failed to destroy instance");
}

std::shared_ptr<vk::PhysicalDevice> LowVK::GetPhysical()
{
	return mPhysicalDevice;
}

std::shared_ptr<vk::Device> LowVK::GetDevice()
{
	return mDevice;
}

vk::SurfaceKHR LowVK::GetSurface()
{
	return mSurface;
}

void LowVK::SetupDebugCallback()
{
	vk::DebugUtilsMessengerCreateInfoEXT debugUtilsInfo;
	debugUtilsInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
	debugUtilsInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
	debugUtilsInfo.pfnUserCallback = debugCallback;
	debugUtilsInfo.pUserData = nullptr; // Optional
	
	CHECK_EX(mCallback = mInstance.createDebugUtilsMessengerEXT(debugUtilsInfo, nullptr, mDldy), "Failed to create debug utils messenger");
}

void LowVK::CreateSurface(void* winCreateFunc)
{
#ifdef WIN32
	vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.hwnd = (HWND)winCreateFunc;
	surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);

	CHECK_EX(mSurface = mInstance.createWin32SurfaceKHR(surfaceCreateInfo), "Failed to create window surface");
#else
	VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
	surfaceCreateInfo.pNext = nullptr;
	surfaceCreateInfo.pView = view;
	surfaceCreateInfo.flags = 0;

	auto CreateMacOSSurfaceKHR = (PFN_vkCreateMacOSSurfaceMVK)vkGetInstanceProcAddr(VkInstance(instance), "vkCreateMacOSSurfaceMVK");

	if (!CreateMacOSSurfaceKHR || CreateMacOSSurfaceKHR(VkInstance(instance), &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
#endif
}