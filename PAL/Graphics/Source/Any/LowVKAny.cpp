#include <PAL/Graphics/LowVK.h>
#include "LayersExtensions.inl"
#include <PAL/Graphics/VKAny.h>

#ifdef WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h>
#define GetProcAddress dlsym
#endif

void* LowVK::mVulkanLibrary{ nullptr };
VkInstance LowVK::mInstance;
VkDebugUtilsMessengerEXT LowVK::mCallback;
VkSurfaceKHR LowVK::mSurface;
std::shared_ptr<VkPhysicalDevice> LowVK::mPhysicalDevice;
std::shared_ptr<VkDevice> LowVK::mDevice;
VkPhysicalDeviceProperties LowVK::mPhysicalDeviceProperties;
std::vector<VkQueueFamilyProperties> LowVK::mQueueFamilyProperties;

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
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << extensions.size() << " instance extensions available: " << std::endl;

		for (size_t idx = 0; idx < extensions.size(); ++idx)
		{
			std::cout << extensions[idx].extensionName << "\n";
		}
	}

	void QueryInstanceLayers()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());


		std::cout << availableLayers.size() << " instance layers available: " << std::endl;

		for (const auto& layer : availableLayers)
		{
			std::cout << layer.layerName << "\n";
		}
	}

	void QueryDeviceExtensions(const VkPhysicalDevice& device)
	{
		std::cout << "Device extensions: " << std::endl;

		uint32_t propertyCount{ 0 };
		VK_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, nullptr));

		std::vector<VkExtensionProperties> deviceExtensions(propertyCount);
		VK_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, deviceExtensions.data()));

		for (const auto& ext : deviceExtensions)
		{
			std::cout << ext.extensionName << ", v: " << VK_VERSION_MAJOR(ext.specVersion) << "."
				<< VK_VERSION_MAJOR(ext.specVersion) << "."
				<< VK_VERSION_PATCH(ext.specVersion) << std::endl;
		}
	}
}

#ifdef __cplusplus
extern "C" {
#endif

#if defined(VK_VERSION_1_0)
    PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
    PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
    PFN_vkAllocateMemory vkAllocateMemory;
    PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
    PFN_vkBindBufferMemory vkBindBufferMemory;
    PFN_vkBindImageMemory vkBindImageMemory;
    PFN_vkCmdBeginQuery vkCmdBeginQuery;
    PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
    PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
    PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
    PFN_vkCmdBindPipeline vkCmdBindPipeline;
    PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
    PFN_vkCmdBlitImage vkCmdBlitImage;
    PFN_vkCmdClearAttachments vkCmdClearAttachments;
    PFN_vkCmdClearColorImage vkCmdClearColorImage;
    PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
    PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
    PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
    PFN_vkCmdCopyImage vkCmdCopyImage;
    PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
    PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
    PFN_vkCmdDispatch vkCmdDispatch;
    PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
    PFN_vkCmdDraw vkCmdDraw;
    PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
    PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
    PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
    PFN_vkCmdEndQuery vkCmdEndQuery;
    PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
    PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
    PFN_vkCmdFillBuffer vkCmdFillBuffer;
    PFN_vkCmdNextSubpass vkCmdNextSubpass;
    PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
    PFN_vkCmdPushConstants vkCmdPushConstants;
    PFN_vkCmdResetEvent vkCmdResetEvent;
    PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
    PFN_vkCmdResolveImage vkCmdResolveImage;
    PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
    PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
    PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
    PFN_vkCmdSetEvent vkCmdSetEvent;
    PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
    PFN_vkCmdSetScissor vkCmdSetScissor;
    PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
    PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
    PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
    PFN_vkCmdSetViewport vkCmdSetViewport;
    PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
    PFN_vkCmdWaitEvents vkCmdWaitEvents;
    PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
    PFN_vkCreateBuffer vkCreateBuffer;
    PFN_vkCreateBufferView vkCreateBufferView;
    PFN_vkCreateCommandPool vkCreateCommandPool;
    PFN_vkCreateComputePipelines vkCreateComputePipelines;
    PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
    PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
    PFN_vkCreateDevice vkCreateDevice;
    PFN_vkCreateEvent vkCreateEvent;
    PFN_vkCreateFence vkCreateFence;
    PFN_vkCreateFramebuffer vkCreateFramebuffer;
    PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
    PFN_vkCreateImage vkCreateImage;
    PFN_vkCreateImageView vkCreateImageView;
    PFN_vkCreateInstance vkCreateInstance;
    PFN_vkCreatePipelineCache vkCreatePipelineCache;
    PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
    PFN_vkCreateQueryPool vkCreateQueryPool;
    PFN_vkCreateRenderPass vkCreateRenderPass;
    PFN_vkCreateSampler vkCreateSampler;
    PFN_vkCreateSemaphore vkCreateSemaphore;
    PFN_vkCreateShaderModule vkCreateShaderModule;
    PFN_vkDestroyBuffer vkDestroyBuffer;
    PFN_vkDestroyBufferView vkDestroyBufferView;
    PFN_vkDestroyCommandPool vkDestroyCommandPool;
    PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
    PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
    PFN_vkDestroyDevice vkDestroyDevice;
    PFN_vkDestroyEvent vkDestroyEvent;
    PFN_vkDestroyFence vkDestroyFence;
    PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
    PFN_vkDestroyImage vkDestroyImage;
    PFN_vkDestroyImageView vkDestroyImageView;
    PFN_vkDestroyInstance vkDestroyInstance;
    PFN_vkDestroyPipeline vkDestroyPipeline;
    PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
    PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
    PFN_vkDestroyQueryPool vkDestroyQueryPool;
    PFN_vkDestroyRenderPass vkDestroyRenderPass;
    PFN_vkDestroySampler vkDestroySampler;
    PFN_vkDestroySemaphore vkDestroySemaphore;
    PFN_vkDestroyShaderModule vkDestroyShaderModule;
    PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
    PFN_vkEndCommandBuffer vkEndCommandBuffer;
    PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
    PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
    PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
    PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
    PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
    PFN_vkFreeMemory vkFreeMemory;
    PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
    PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
    PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
    PFN_vkGetDeviceQueue vkGetDeviceQueue;
    PFN_vkGetEventStatus vkGetEventStatus;
    PFN_vkGetFenceStatus vkGetFenceStatus;
    PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
    PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
    PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
    PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
    PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
    PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
    PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
    PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
    PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
    PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
    PFN_vkMapMemory vkMapMemory;
    PFN_vkMergePipelineCaches vkMergePipelineCaches;
    PFN_vkQueueBindSparse vkQueueBindSparse;
    PFN_vkQueueSubmit vkQueueSubmit;
    PFN_vkQueueWaitIdle vkQueueWaitIdle;
    PFN_vkResetCommandBuffer vkResetCommandBuffer;
    PFN_vkResetCommandPool vkResetCommandPool;
    PFN_vkResetDescriptorPool vkResetDescriptorPool;
    PFN_vkResetEvent vkResetEvent;
    PFN_vkResetFences vkResetFences;
    PFN_vkSetEvent vkSetEvent;
    PFN_vkUnmapMemory vkUnmapMemory;
    PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
    PFN_vkWaitForFences vkWaitForFences;
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
    PFN_vkBindBufferMemory2 vkBindBufferMemory2;
    PFN_vkBindImageMemory2 vkBindImageMemory2;
    PFN_vkCmdDispatchBase vkCmdDispatchBase;
    PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
    PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
    PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
    PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
    PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
    PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
    PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups;
    PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
    PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
    PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
    PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
    PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
    PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
    PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties;
    PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties;
    PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties;
    PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
    PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2;
    PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2;
    PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
    PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2;
    PFN_vkTrimCommandPool vkTrimCommandPool;
    PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_AMD_buffer_marker)
    PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_draw_indirect_count)
    PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
    PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
    PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
    PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
    PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_ANDROID_native_buffer)
    PFN_vkAcquireImageANDROID vkAcquireImageANDROID;
    PFN_vkGetSwapchainGrallocUsageANDROID vkGetSwapchainGrallocUsageANDROID;
    PFN_vkQueueSignalReleaseImageANDROID vkQueueSignalReleaseImageANDROID;
#endif /* defined(VK_ANDROID_native_buffer) */
#if defined(VK_EXT_acquire_xlib_display)
    PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT;
    PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT;
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_conditional_rendering)
    PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
    PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
    PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
    PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
    PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
    PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
    PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_debug_report)
    PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
    PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
    PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
    PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
    PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
    PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
    PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
    PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
    PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
    PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
    PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
    PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT;
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_discard_rectangles)
    PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_display_control)
    PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
    PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
    PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
    PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_display_surface_counter)
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_external_memory_host)
    PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_hdr_metadata)
    PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_sample_locations)
    PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
    PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_validation_cache)
    PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
    PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
    PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
    PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_GOOGLE_display_timing)
    PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
    PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_KHR_android_surface)
    PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_bind_memory2)
    PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
    PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_create_renderpass2)
    PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
    PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
    PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
    PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_descriptor_update_template)
    PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
    PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
    PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
    PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
    PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
    PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_device_group_creation)
    PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR;
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
    PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
    PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
    PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
    PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_display_swapchain)
    PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
    PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
    PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_external_fence_capabilities)
    PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR;
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_fence_fd)
    PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
    PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
    PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
    PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_capabilities)
    PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_memory_fd)
    PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
    PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
    PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
    PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_capabilities)
    PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_external_semaphore_fd)
    PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
    PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
    PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
    PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_get_display_properties2)
    PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR;
    PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR;
    PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
    PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR;
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_memory_requirements2)
    PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
    PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
    PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_get_physical_device_properties2)
    PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR;
    PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR;
    PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR;
    PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR;
    PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
    PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_maintenance1)
    PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
    PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_mir_surface)
    PFN_vkCreateMirSurfaceKHR vkCreateMirSurfaceKHR;
    PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR;
#endif /* defined(VK_KHR_mir_surface) */
#if defined(VK_KHR_push_descriptor)
    PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
    PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
    PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
    PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_surface)
    PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_swapchain)
    PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_wayland_surface)
    PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
    PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
    PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
    PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
    PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK;
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
    PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
    PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NVX_device_generated_commands)
    PFN_vkCmdProcessCommandsNVX vkCmdProcessCommandsNVX;
    PFN_vkCmdReserveSpaceForCommandsNVX vkCmdReserveSpaceForCommandsNVX;
    PFN_vkCreateIndirectCommandsLayoutNVX vkCreateIndirectCommandsLayoutNVX;
    PFN_vkCreateObjectTableNVX vkCreateObjectTableNVX;
    PFN_vkDestroyIndirectCommandsLayoutNVX vkDestroyIndirectCommandsLayoutNVX;
    PFN_vkDestroyObjectTableNVX vkDestroyObjectTableNVX;
    PFN_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX;
    PFN_vkRegisterObjectsNVX vkRegisterObjectsNVX;
    PFN_vkUnregisterObjectsNVX vkUnregisterObjectsNVX;
#endif /* defined(VK_NVX_device_generated_commands) */
#if defined(VK_NV_clip_space_w_scaling)
    PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_device_diagnostic_checkpoints)
    PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
    PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_external_memory_capabilities)
    PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_external_memory_win32)
    PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif /* defined(VK_NV_external_memory_win32) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1))
    PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
    PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
    PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
    PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
    PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */

#ifdef __cplusplus
}
#endif

void LowVK::Initialize(const std::string& appName, void* winCreateFunc)
{
#if defined(WIN32) || defined(_WIN32)
	mVulkanLibrary = LoadLibrary("vulkan-1.dll");
#else
    mVulkanLibrary = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
#endif
    
    if (!mVulkanLibrary)
    {
        std::cout << "Could not load Vulkan library!" << std::endl;
        return;
    }

    LoadGlobalFunctions();

	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	appInfo.pNext = nullptr;

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(mEnabledInstanceExtensions.size());
	instanceInfo.ppEnabledExtensionNames = mEnabledInstanceExtensions.data();
	instanceInfo.enabledLayerCount = static_cast<uint32_t>(mEnabledInstanceValidationLayers.size());
	instanceInfo.ppEnabledLayerNames = mEnabledInstanceValidationLayers.data();
	instanceInfo.flags = 0;
	instanceInfo.pNext = nullptr;

	VK_CHECK_RESULT(vkCreateInstance(&instanceInfo, nullptr, &mInstance));

	LoadInstanceFunctions();
    LoadInstanceExtensions();

	QueryInstanceLayers();
	QueryInstanceExtensions();

	if (!mEnabledInstanceValidationLayers.empty())
	{
		SetupDebugCallback();
	}

	uint32_t deviceCount{ 0 };
	VK_CHECK_RESULT(vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr));

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	VK_CHECK_RESULT(vkEnumeratePhysicalDevices(mInstance, &deviceCount, physicalDevices.data()));

	std::cout << "Available GPU devices: " << physicalDevices.size() << std::endl;

	for (const auto& device : physicalDevices)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		std::cout << deviceProperties.deviceName
			<< ", supported vulkan API version: "
			<< VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
			<< VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
			<< VK_VERSION_PATCH(deviceProperties.apiVersion) << std::endl;
	}

	mPhysicalDevice = std::make_shared<VkPhysicalDevice>(physicalDevices.back());

	QueryDeviceExtensions(*mPhysicalDevice);

	CreateSurface(winCreateFunc);

	uint32_t queuePropertyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(*mPhysicalDevice, &queuePropertyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueProperties(queuePropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(*mPhysicalDevice, &queuePropertyCount, queueProperties.data()); // this has to be called before creating queues

	std::vector<float> queuePriorities{ 1.0f };

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueCount = static_cast<uint32_t>(queuePriorities.size());
	queueCreateInfo.queueFamilyIndex = 0;
	queueCreateInfo.pQueuePriorities = queuePriorities.data();

	VkPhysicalDeviceFeatures deviceFeatures{};
	vkGetPhysicalDeviceFeatures(*mPhysicalDevice.get(), &deviceFeatures);
	vkGetPhysicalDeviceProperties(*mPhysicalDevice.get(), &mPhysicalDeviceProperties);

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(mEnabledDeviceExtensions.size());;
	deviceCreateInfo.ppEnabledExtensionNames = mEnabledDeviceExtensions.data();
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mEnabledDeviceValidationLayers.size());
	deviceCreateInfo.ppEnabledLayerNames = mEnabledDeviceValidationLayers.data();

	mDevice = std::make_shared<VkDevice>();
	VK_CHECK_RESULT(vkCreateDevice(*mPhysicalDevice.get(), &deviceCreateInfo, nullptr, mDevice.get()));

	LoadDeviceFunctions();
    
    
}

void LowVK::Deinitialize()
{
	vkDestroyDevice(*mDevice.get(), nullptr);
	vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	vkDestroyDebugUtilsMessengerEXT(mInstance, mCallback, nullptr);
	vkDestroyInstance(mInstance, nullptr);
}

std::shared_ptr<VkPhysicalDevice> LowVK::GetPhysical()
{
	return mPhysicalDevice;
}

std::shared_ptr<VkDevice> LowVK::GetDevice()
{
	return mDevice;
}

VkSurfaceKHR LowVK::GetSurface()
{
	return mSurface;
}

VkPhysicalDeviceProperties& LowVK::GetPhysicalDeviceProperties()
{
	return mPhysicalDeviceProperties;
}

std::vector<VkQueueFamilyProperties>& LowVK::GetPhysicalDeviceQueueFamilyProperties()
{
	uint32_t queueFamilyPropertyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(*mPhysicalDevice.get(), &queueFamilyPropertyCount, nullptr);
	mQueueFamilyProperties.resize(queueFamilyPropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(*mPhysicalDevice.get(), &queueFamilyPropertyCount, mQueueFamilyProperties.data());

	return mQueueFamilyProperties;
}

void LowVK::GetPhysicalDeviceFormatProperties(VkFormat format, VkFormatProperties* pFormatProperties)
{
	vkGetPhysicalDeviceFormatProperties(*mPhysicalDevice.get(), format, pFormatProperties);
}

void LowVK::GetBufferMemoryRequirements(const VkBuffer& buffer, VkMemoryRequirements * pMemoryRequirements)
{
	vkGetBufferMemoryRequirements(*mDevice.get(), buffer, pMemoryRequirements);
}

void LowVK::GetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
	vkGetImageMemoryRequirements(*mDevice.get(), image, pMemoryRequirements);
}

void LowVK::GetPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties* pMemoryProperties)
{
	vkGetPhysicalDeviceMemoryProperties(*mPhysicalDevice.get(), pMemoryProperties);
}

void LowVK::GetPhysicalDeviceSurfaceCapabilitiesKHR(VkSurfaceCapabilitiesKHR * pSurfaceCapabilities)
{
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*mPhysicalDevice.get(), mSurface, pSurfaceCapabilities));
}

void LowVK::GetPhysicalDeviceSurfaceFormatsKHR(std::vector<VkSurfaceFormatKHR>& surfaceFormats)
{
	uint32_t surfaceFormatCount{ 0 };
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(*mPhysicalDevice.get(), mSurface, &surfaceFormatCount, nullptr));
	surfaceFormats.resize(surfaceFormatCount);
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(*mPhysicalDevice.get(), mSurface, &surfaceFormatCount, surfaceFormats.data()));
}

void LowVK::GetPhysicalDeviceSurfacePresentModesKHR(std::vector<VkPresentModeKHR>& pPresentModes)
{
	uint32_t presentationModesCount{ 0 };
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(*mPhysicalDevice.get(), mSurface, &presentationModesCount, nullptr));
	pPresentModes.resize(presentationModesCount);
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(*mPhysicalDevice.get(), mSurface, &presentationModesCount, pPresentModes.data()));
}

void LowVK::GetPhysicalDeviceSurfaceSupportKHR(uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32 * pSupported)
{
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(*mPhysicalDevice.get(), queueFamilyIndex, surface, pSupported));
}

void LowVK::GetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
	vkGetDeviceQueue(*mDevice.get(), 0, 0, pQueue);
}

void LowVK::CreateCommandPool(const VkCommandPoolCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkCommandPool * pCommandPool)
{
	VK_CHECK_RESULT(vkCreateCommandPool(*mDevice.get(), pCreateInfo, pAllocator, pCommandPool));
}

void LowVK::DeviceWaitIdle()
{
	VK_CHECK_RESULT(vkDeviceWaitIdle(*mDevice.get()));
}

void LowVK::CreateFramebuffer(const VkFramebufferCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkFramebuffer * pFramebuffer)
{
	VK_CHECK_RESULT(vkCreateFramebuffer(*mDevice.get(), pCreateInfo, pAllocator, pFramebuffer));
}

GRAPHICS_API void LowVK::DestroyFramebuffer(VkFramebuffer framebuffer, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyFramebuffer(*mDevice.get(), framebuffer, pAllocator);
}

void LowVK::CreateImage(const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
	VK_CHECK_RESULT(vkCreateImage(*mDevice.get(), pCreateInfo, pAllocator, pImage));
}

void LowVK::DestroyImage(VkImage image, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyImage(*mDevice.get(), image, pAllocator);
}

void LowVK::CreateImageView(const VkImageViewCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkImageView * pView)
{
	VK_CHECK_RESULT(vkCreateImageView(*mDevice.get(), pCreateInfo, pAllocator, pView));
}

void LowVK::CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
	VK_CHECK_RESULT(vkCreateDescriptorSetLayout(*mDevice.get(), pCreateInfo, pAllocator, pSetLayout));
}

void LowVK::DestroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyDescriptorSetLayout(*mDevice.get(), descriptorSetLayout, pAllocator);
}

void LowVK::AllocateDescriptorSets(const VkDescriptorSetAllocateInfo * pAllocateInfo, VkDescriptorSet * pDescriptorSets)
{
	VK_CHECK_RESULT(vkAllocateDescriptorSets(*mDevice.get(), pAllocateInfo, pDescriptorSets));
}

void LowVK::CreateDescriptorPool(const VkDescriptorPoolCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDescriptorPool * pDescriptorPool)
{
	VK_CHECK_RESULT(vkCreateDescriptorPool(*mDevice.get(), pCreateInfo, pAllocator, pDescriptorPool));
}

void LowVK::DestroyDescriptorPool(VkDescriptorPool descriptorPool, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyDescriptorPool(*mDevice.get(), descriptorPool, pAllocator);
}

void LowVK::UpdateDescriptorSets(const std::vector<VkWriteDescriptorSet>& descriptorWrites, const std::vector<VkCopyDescriptorSet>& descriptorCopies)
{
	vkUpdateDescriptorSets(*mDevice.get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), static_cast<uint32_t>(descriptorCopies.size()), descriptorCopies.data());
}

void LowVK::CreatePipelineLayout(const VkPipelineLayoutCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkPipelineLayout * pPipelineLayout)
{
	VK_CHECK_RESULT(vkCreatePipelineLayout(*mDevice.get(), pCreateInfo, pAllocator, pPipelineLayout));
}

void LowVK::CreateGraphicsPipelines(VkPipelineCache pipelineCache, const std::vector<VkGraphicsPipelineCreateInfo>& createInfos, const VkAllocationCallbacks * pAllocator, VkPipeline * pPipelines)
{
	VK_CHECK_RESULT(vkCreateGraphicsPipelines(*mDevice.get(), pipelineCache, static_cast<uint32_t>(createInfos.size()), createInfos.data(), pAllocator, pPipelines));
}

void LowVK::CreateSemaphores(const VkSemaphoreCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkSemaphore * pSemaphore)
{
	VK_CHECK_RESULT(vkCreateSemaphore(*mDevice.get(), pCreateInfo, pAllocator, pSemaphore));
}

void LowVK::CreateRenderPass(const VkRenderPassCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkRenderPass * pRenderPass)
{
	VK_CHECK_RESULT(vkCreateRenderPass(*mDevice.get(), pCreateInfo, pAllocator, pRenderPass));
}

void LowVK::DestroyRenderPass(VkRenderPass renderPass, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyRenderPass(*mDevice.get(), renderPass, pAllocator);
}

void LowVK::CreateShaderModule(const VkShaderModuleCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkShaderModule * pShaderModule)
{
	VK_CHECK_RESULT(vkCreateShaderModule(*mDevice.get(), pCreateInfo, pAllocator, pShaderModule));
}

void LowVK::DestroyShaderModule(VkShaderModule shaderModule, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyShaderModule(*mDevice.get(), shaderModule, pAllocator);
}

void LowVK::CreateBuffer(const VkBufferCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkBuffer * pBuffer)
{
	VK_CHECK_RESULT(vkCreateBuffer(*mDevice.get(), pCreateInfo, pAllocator, pBuffer));
}

void LowVK::DestroyBuffer(VkBuffer buffer, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyBuffer(*mDevice.get(), buffer, pAllocator);
}

void LowVK::CreateSampler(const VkSamplerCreateInfo * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkSampler * pSampler)
{
	VK_CHECK_RESULT(vkCreateSampler(*mDevice.get(), pCreateInfo, pAllocator, pSampler));
}

void LowVK::DestroySampler(VkSampler sampler, const VkAllocationCallbacks * pAllocator)
{
	vkDestroySampler(*mDevice.get(), sampler, pAllocator);
}

void LowVK::MapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void **ppData)
{
	VK_CHECK_RESULT(vkMapMemory(*mDevice.get(), memory, offset, size, flags, ppData));
}

void LowVK::UnmapMemory(VkDeviceMemory memory)
{
	vkUnmapMemory(*mDevice.get(), memory);
}

void LowVK::FreeMemory(VkDeviceMemory memory, const VkAllocationCallbacks * pAllocator)
{
	vkFreeMemory(*mDevice.get(), memory, pAllocator);
}

void LowVK::CreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain)
{
	VK_CHECK_RESULT(vkCreateSwapchainKHR(*mDevice.get(), pCreateInfo, pAllocator, pSwapchain));
}

void LowVK::DestroySwapchainKHR(VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
	vkDestroySwapchainKHR(*mDevice.get(), swapchain, pAllocator);
}

void LowVK::GetSwapchainImagesKHR(VkSwapchainKHR swapchain, std::vector<VkImage>& images)
{
	uint32_t swapChainImageCount{ 0 };
	VK_CHECK_RESULT(vkGetSwapchainImagesKHR(*mDevice.get(), swapchain, &swapChainImageCount, nullptr));
	images.resize(swapChainImageCount);
	VK_CHECK_RESULT(vkGetSwapchainImagesKHR(*mDevice.get(), swapchain, &swapChainImageCount, images.data()));
}

void LowVK::AcquireNextImageKHR(VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t * pImageIndex)
{
	VK_CHECK_RESULT(vkAcquireNextImageKHR(*mDevice.get(), swapchain, timeout, semaphore, fence, pImageIndex));
}

void LowVK::QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR * pPresentInfo)
{
	VK_CHECK_RESULT(vkQueuePresentKHR(queue, pPresentInfo));
}

void LowVK::DestroyCommandPool(VkCommandPool commandPool, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyCommandPool(*mDevice.get(), commandPool, pAllocator);
}

void LowVK::DestroyImageView(VkImageView imageView, const VkAllocationCallbacks * pAllocator)
{
	vkDestroyImageView(*mDevice.get(), imageView, pAllocator);
}

void LowVK::AllocateMemory(const VkMemoryAllocateInfo * pAllocateInfo, const VkAllocationCallbacks * pAllocator, VkDeviceMemory * pMemory)
{
	VK_CHECK_RESULT(vkAllocateMemory(*mDevice.get(), pAllocateInfo, pAllocator, pMemory));
}

void LowVK::BindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
	VK_CHECK_RESULT(vkBindBufferMemory(*mDevice.get(), buffer, memory, memoryOffset));
}

void LowVK::BindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
	VK_CHECK_RESULT(vkBindImageMemory(*mDevice.get(), image, memory, memoryOffset));
}

void LowVK::CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, const std::vector<VkBufferImageCopy>& regions)
{
	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, static_cast<uint32_t>(regions.size()), regions.data());
}

void LowVK::CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, const std::vector<VkBufferCopy>& regions)
{
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, static_cast<uint32_t>(regions.size()), regions.data());
}

void LowVK::CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
	vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}

void LowVK::CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, const std::vector<VkDescriptorSet>& descriptorSets, const std::vector<uint32_t>& dynamicOffsets)
{
	vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), static_cast<uint32_t>(dynamicOffsets.size()), dynamicOffsets.data());
}

void LowVK::CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
	vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}

void LowVK::CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer * pBuffers, const VkDeviceSize * pOffsets)
{
	vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void LowVK::CmdEndRenderPass(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

void LowVK::CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, const std::vector<VkViewport>& viewports)
{
	vkCmdSetViewport(commandBuffer, firstViewport, static_cast<uint32_t>(viewports.size()), viewports.data());
}

void LowVK::CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo * pRenderPassBegin, VkSubpassContents contents)
{
	vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}

void LowVK::AllocateCommandBuffers(const VkCommandBufferAllocateInfo * pAllocateInfo, VkCommandBuffer * pCommandBuffers)
{
	VK_CHECK_RESULT(vkAllocateCommandBuffers(*mDevice.get(), pAllocateInfo, pCommandBuffers));
}

void LowVK::FreeCommandBuffers(VkCommandPool commandPool, const std::vector<VkCommandBuffer>& commandBuffers)
{
	vkFreeCommandBuffers(*mDevice.get(), commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

void LowVK::BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo * pBeginInfo)
{
	VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, pBeginInfo));
}

void LowVK::EndCommandBuffer(VkCommandBuffer commandBuffer)
{
	VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
}

void LowVK::ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags)
{
	VK_CHECK_RESULT(vkResetCommandBuffer(commandBuffer, flags));
}

void LowVK::CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, const std::vector<VkMemoryBarrier>& memoryBarriers, const std::vector<VkBufferMemoryBarrier>& bufferMemoryBarriers, const std::vector<VkImageMemoryBarrier>& imageMemoryBarriers)
{
	vkCmdPipelineBarrier(
		commandBuffer,
		srcStageMask, dstStageMask,
		dependencyFlags,
		static_cast<uint32_t>(memoryBarriers.size()), memoryBarriers.data(),
		static_cast<uint32_t>(bufferMemoryBarriers.size()), bufferMemoryBarriers.data(),
		static_cast<uint32_t>(imageMemoryBarriers.size()), imageMemoryBarriers.data());
}

void LowVK::QueueWaitIdle(const VkQueue& queue)
{
	VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}

void LowVK::QueueSubmit(const VkQueue& queue, const std::vector<VkSubmitInfo>& submits, const VkFence& fence)
{
	VK_CHECK_RESULT(vkQueueSubmit(queue, static_cast<uint32_t>(submits.size()), submits.data(), fence));
}

void LowVK::CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
	vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void LowVK::CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void LowVK::SetupDebugCallback()
{
	VkDebugUtilsMessengerCreateInfoEXT debugUtilsInfo;
	debugUtilsInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilsInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
	debugUtilsInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	debugUtilsInfo.pfnUserCallback = debugCallback;
	debugUtilsInfo.pUserData = nullptr; // Optional
	debugUtilsInfo.flags = 0;
	
	VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(mInstance, &debugUtilsInfo, nullptr, &mCallback));
}

void LowVK::CreateSurface(void* winCreateFunc)
{
#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hwnd = (HWND)winCreateFunc;
	surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
	surfaceCreateInfo.pNext = nullptr;
	surfaceCreateInfo.flags = 0;

	VK_CHECK_RESULT(vkCreateWin32SurfaceKHR(mInstance, &surfaceCreateInfo, nullptr, &mSurface));
#else
    VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo{};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
	surfaceCreateInfo.pNext = nullptr;
	surfaceCreateInfo.pView = winCreateFunc;
	surfaceCreateInfo.flags = 0;
    
    VK_CHECK_RESULT(vkCreateMacOSSurfaceMVK(mInstance, &surfaceCreateInfo, nullptr, &mSurface));
#endif
}

void LowVK::LoadGlobalFunctions()
{
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(mVulkanLibrary, "vkGetInstanceProcAddr");
    
    vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(nullptr, "vkCreateInstance");
    vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceExtensionProperties");
    vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceLayerProperties");
}

void LowVK::LoadInstanceFunctions()
{
#if defined(VK_VERSION_1_1)
    vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion");
#endif

#if defined(VK_VERSION_1_0)
    vkCreateDevice = (PFN_vkCreateDevice)vkGetInstanceProcAddr(mInstance, "vkCreateDevice");
    vkDestroyInstance = (PFN_vkDestroyInstance)vkGetInstanceProcAddr(mInstance, "vkDestroyInstance");
    vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)vkGetInstanceProcAddr(mInstance, "vkEnumerateDeviceExtensionProperties");
    vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)vkGetInstanceProcAddr(mInstance, "vkEnumerateDeviceLayerProperties");
    vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)vkGetInstanceProcAddr(mInstance, "vkEnumeratePhysicalDevices");
    vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vkGetInstanceProcAddr(mInstance, "vkGetDeviceProcAddr");
    vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)vkGetInstanceProcAddr(mInstance, "vkGetPhysicalDeviceFeatures");
    vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)vkGetInstanceProcAddr(mInstance, "vkGetPhysicalDeviceFormatProperties");
    vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)vkGetInstanceProcAddr(mInstance, "vkGetPhysicalDeviceImageFormatProperties");
    vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)vkGetInstanceProcAddr(mInstance, "vkGetPhysicalDeviceMemoryProperties");
    vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)vkGetInstanceProcAddr(mInstance, "vkGetPhysicalDeviceProperties");
    vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)vkGetInstanceProcAddr(mInstance, "vkGetPhysicalDeviceQueueFamilyProperties");
    vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)vkGetInstanceProcAddr(mInstance, "vkGetPhysicalDeviceSparseImageFormatProperties");
#endif
}

#define VK_INSTANCE_LEVEL_FUNCTION(instance, fun)											\
if(!(fun = (PFN_##fun)vkGetInstanceProcAddr( instance, #fun )))								\
{																							\
	std::cout << "Could not load instance level function: " << #fun << "!" << std::endl;	\
	fun = nullptr;																			\
}	

void LowVK::LoadInstanceExtensions()
{
    // VK_KHR_win32_surface)
#if defined(WIN32)
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkCreateWin32SurfaceKHR);
#else
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkCreateMacOSSurfaceMVK);
#endif
    
    // VK_EXT_debug_utils
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkCreateDebugUtilsMessengerEXT);
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkDestroyDebugUtilsMessengerEXT);

    // VK_KHR_surface
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkDestroySurfaceKHR);
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkGetPhysicalDeviceSurfaceFormatsKHR);
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkGetPhysicalDeviceSurfacePresentModesKHR);
    VK_INSTANCE_LEVEL_FUNCTION(mInstance, vkGetPhysicalDeviceSurfaceSupportKHR);
}

#define VK_DEVICE_LEVEL_FUNCTION(device, fun)											\
if(!(fun = (PFN_##fun)vkGetDeviceProcAddr( device, #fun )))								\
{																						\
  std::cout << "Could not load device level function: " << #fun << "!" << std::endl;	\
  fun = nullptr;																		\
}																						\

void LowVK::LoadDeviceFunctions()
{
	if (!mDevice)
	{
		std::cout << "Cannot load vulkan device functions, create device first!" << std::endl;
		return;
	}

    const auto& device = *mDevice.get();

    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyDevice);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyCommandPool);
    VK_DEVICE_LEVEL_FUNCTION(device, vkGetDeviceQueue);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateShaderModule);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyShaderModule);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateBuffer);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyBuffer);

    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateFramebuffer);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyFramebuffer);

    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateRenderPass);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyRenderPass);

    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateGraphicsPipelines);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateImage);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyImage);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateImageView);
    VK_DEVICE_LEVEL_FUNCTION(device, vkAllocateMemory);
    VK_DEVICE_LEVEL_FUNCTION(device, vkBindImageMemory);
    VK_DEVICE_LEVEL_FUNCTION(device, vkBindBufferMemory);
    VK_DEVICE_LEVEL_FUNCTION(device, vkMapMemory);
    VK_DEVICE_LEVEL_FUNCTION(device, vkUnmapMemory);
    VK_DEVICE_LEVEL_FUNCTION(device, vkFreeMemory);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreatePipelineLayout);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateDescriptorSetLayout);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyDescriptorSetLayout);
    VK_DEVICE_LEVEL_FUNCTION(device, vkUpdateDescriptorSets);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateDescriptorPool);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyDescriptorPool);
    VK_DEVICE_LEVEL_FUNCTION(device, vkAllocateDescriptorSets);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateCommandPool);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateSampler);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroySampler);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateSwapchainKHR);
    VK_DEVICE_LEVEL_FUNCTION(device, vkGetSwapchainImagesKHR);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDeviceWaitIdle);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroyImageView);
    VK_DEVICE_LEVEL_FUNCTION(device, vkDestroySwapchainKHR);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCreateSemaphore);

    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdBeginRenderPass);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdEndRenderPass);

    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdBindPipeline);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdBindIndexBuffer);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdBindVertexBuffers);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdBindDescriptorSets);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdSetViewport);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdDraw);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdDrawIndexed);


    VK_DEVICE_LEVEL_FUNCTION(device, vkFreeCommandBuffers);
    VK_DEVICE_LEVEL_FUNCTION(device, vkAllocateCommandBuffers);
    VK_DEVICE_LEVEL_FUNCTION(device, vkBeginCommandBuffer);
    VK_DEVICE_LEVEL_FUNCTION(device, vkEndCommandBuffer);
    VK_DEVICE_LEVEL_FUNCTION(device, vkResetCommandBuffer);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdPipelineBarrier);

    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdCopyBuffer);
    VK_DEVICE_LEVEL_FUNCTION(device, vkCmdCopyBufferToImage);
    VK_DEVICE_LEVEL_FUNCTION(device, vkGetImageMemoryRequirements);
    VK_DEVICE_LEVEL_FUNCTION(device, vkGetBufferMemoryRequirements);

    VK_DEVICE_LEVEL_FUNCTION(device, vkAcquireNextImageKHR);
    VK_DEVICE_LEVEL_FUNCTION(device, vkQueueWaitIdle);
    VK_DEVICE_LEVEL_FUNCTION(device, vkQueueSubmit);
    VK_DEVICE_LEVEL_FUNCTION(device, vkQueuePresentKHR);
}
