#pragma once

#ifndef VK_NO_PROTOTYPES
#define VK_NO_PROTOTYPES
#endif
#include <vulkan/vulkan.h>

#include <PAL/Graphics/GraphicsBase.h>


#ifdef __cplusplus
extern "C" {
#endif

#if defined(VK_VERSION_1_0)
		GRAPHICS_API extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
		GRAPHICS_API extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
		GRAPHICS_API extern PFN_vkAllocateMemory vkAllocateMemory;
		GRAPHICS_API extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
		GRAPHICS_API extern PFN_vkBindBufferMemory vkBindBufferMemory;
		GRAPHICS_API extern PFN_vkBindImageMemory vkBindImageMemory;
		GRAPHICS_API extern PFN_vkCmdBeginQuery vkCmdBeginQuery;
		GRAPHICS_API extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
		GRAPHICS_API extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
		GRAPHICS_API extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
		GRAPHICS_API extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
		GRAPHICS_API extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
		GRAPHICS_API extern PFN_vkCmdBlitImage vkCmdBlitImage;
		GRAPHICS_API extern PFN_vkCmdClearAttachments vkCmdClearAttachments;
		GRAPHICS_API extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
		GRAPHICS_API extern PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
		GRAPHICS_API extern PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
		GRAPHICS_API extern PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
		GRAPHICS_API extern PFN_vkCmdCopyImage vkCmdCopyImage;
		GRAPHICS_API extern PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
		GRAPHICS_API extern PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
		GRAPHICS_API extern PFN_vkCmdDispatch vkCmdDispatch;
		GRAPHICS_API extern PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
		GRAPHICS_API extern PFN_vkCmdDraw vkCmdDraw;
		GRAPHICS_API extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
		GRAPHICS_API extern PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
		GRAPHICS_API extern PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
		GRAPHICS_API extern PFN_vkCmdEndQuery vkCmdEndQuery;
		GRAPHICS_API extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
		GRAPHICS_API extern PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
		GRAPHICS_API extern PFN_vkCmdFillBuffer vkCmdFillBuffer;
		GRAPHICS_API extern PFN_vkCmdNextSubpass vkCmdNextSubpass;
		GRAPHICS_API extern PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
		GRAPHICS_API extern PFN_vkCmdPushConstants vkCmdPushConstants;
		GRAPHICS_API extern PFN_vkCmdResetEvent vkCmdResetEvent;
		GRAPHICS_API extern PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
		GRAPHICS_API extern PFN_vkCmdResolveImage vkCmdResolveImage;
		GRAPHICS_API extern PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
		GRAPHICS_API extern PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
		GRAPHICS_API extern PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
		GRAPHICS_API extern PFN_vkCmdSetEvent vkCmdSetEvent;
		GRAPHICS_API extern PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
		GRAPHICS_API extern PFN_vkCmdSetScissor vkCmdSetScissor;
		GRAPHICS_API extern PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
		GRAPHICS_API extern PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
		GRAPHICS_API extern PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
		GRAPHICS_API extern PFN_vkCmdSetViewport vkCmdSetViewport;
		GRAPHICS_API extern PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
		GRAPHICS_API extern PFN_vkCmdWaitEvents vkCmdWaitEvents;
		GRAPHICS_API extern PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
		GRAPHICS_API extern PFN_vkCreateBuffer vkCreateBuffer;
		GRAPHICS_API extern PFN_vkCreateBufferView vkCreateBufferView;
		GRAPHICS_API extern PFN_vkCreateCommandPool vkCreateCommandPool;
		GRAPHICS_API extern PFN_vkCreateComputePipelines vkCreateComputePipelines;
		GRAPHICS_API extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
		GRAPHICS_API extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
		GRAPHICS_API extern PFN_vkCreateDevice vkCreateDevice;
		GRAPHICS_API extern PFN_vkCreateEvent vkCreateEvent;
		GRAPHICS_API extern PFN_vkCreateFence vkCreateFence;
		GRAPHICS_API extern PFN_vkCreateFramebuffer vkCreateFramebuffer;
		GRAPHICS_API extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
		GRAPHICS_API extern PFN_vkCreateImage vkCreateImage;
		GRAPHICS_API extern PFN_vkCreateImageView vkCreateImageView;
		GRAPHICS_API extern PFN_vkCreateInstance vkCreateInstance;
		GRAPHICS_API extern PFN_vkCreatePipelineCache vkCreatePipelineCache;
		GRAPHICS_API extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
		GRAPHICS_API extern PFN_vkCreateQueryPool vkCreateQueryPool;
		GRAPHICS_API extern PFN_vkCreateRenderPass vkCreateRenderPass;
		GRAPHICS_API extern PFN_vkCreateSampler vkCreateSampler;
		GRAPHICS_API extern PFN_vkCreateSemaphore vkCreateSemaphore;
		GRAPHICS_API extern PFN_vkCreateShaderModule vkCreateShaderModule;
		GRAPHICS_API extern PFN_vkDestroyBuffer vkDestroyBuffer;
		GRAPHICS_API extern PFN_vkDestroyBufferView vkDestroyBufferView;
		GRAPHICS_API extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
		GRAPHICS_API extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
		GRAPHICS_API extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
		GRAPHICS_API extern PFN_vkDestroyDevice vkDestroyDevice;
		GRAPHICS_API extern PFN_vkDestroyEvent vkDestroyEvent;
		GRAPHICS_API extern PFN_vkDestroyFence vkDestroyFence;
		GRAPHICS_API extern PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
		GRAPHICS_API extern PFN_vkDestroyImage vkDestroyImage;
		GRAPHICS_API extern PFN_vkDestroyImageView vkDestroyImageView;
		GRAPHICS_API extern PFN_vkDestroyInstance vkDestroyInstance;
		GRAPHICS_API extern PFN_vkDestroyPipeline vkDestroyPipeline;
		GRAPHICS_API extern PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
		GRAPHICS_API extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
		GRAPHICS_API extern PFN_vkDestroyQueryPool vkDestroyQueryPool;
		GRAPHICS_API extern PFN_vkDestroyRenderPass vkDestroyRenderPass;
		GRAPHICS_API extern PFN_vkDestroySampler vkDestroySampler;
		GRAPHICS_API extern PFN_vkDestroySemaphore vkDestroySemaphore;
		GRAPHICS_API extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
		GRAPHICS_API extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
		GRAPHICS_API extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
		GRAPHICS_API extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
		GRAPHICS_API extern PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
		GRAPHICS_API extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
		GRAPHICS_API extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
		GRAPHICS_API extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
		GRAPHICS_API extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
		GRAPHICS_API extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
		GRAPHICS_API extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
		GRAPHICS_API extern PFN_vkFreeMemory vkFreeMemory;
		GRAPHICS_API extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
		GRAPHICS_API extern PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
		GRAPHICS_API extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
		GRAPHICS_API extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
		GRAPHICS_API extern PFN_vkGetEventStatus vkGetEventStatus;
		GRAPHICS_API extern PFN_vkGetFenceStatus vkGetFenceStatus;
		GRAPHICS_API extern PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
		GRAPHICS_API extern PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
		GRAPHICS_API extern PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
		GRAPHICS_API extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
		GRAPHICS_API extern PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
		GRAPHICS_API extern PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
		GRAPHICS_API extern PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
		GRAPHICS_API extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
		GRAPHICS_API extern PFN_vkMapMemory vkMapMemory;
		GRAPHICS_API extern PFN_vkMergePipelineCaches vkMergePipelineCaches;
		GRAPHICS_API extern PFN_vkQueueBindSparse vkQueueBindSparse;
		GRAPHICS_API extern PFN_vkQueueSubmit vkQueueSubmit;
		GRAPHICS_API extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
		GRAPHICS_API extern PFN_vkResetCommandBuffer vkResetCommandBuffer;
		GRAPHICS_API extern PFN_vkResetCommandPool vkResetCommandPool;
		GRAPHICS_API extern PFN_vkResetDescriptorPool vkResetDescriptorPool;
		GRAPHICS_API extern PFN_vkResetEvent vkResetEvent;
		GRAPHICS_API extern PFN_vkResetFences vkResetFences;
		GRAPHICS_API extern PFN_vkSetEvent vkSetEvent;
		GRAPHICS_API extern PFN_vkUnmapMemory vkUnmapMemory;
		GRAPHICS_API extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
		GRAPHICS_API extern PFN_vkWaitForFences vkWaitForFences;
#endif

#if defined(VK_VERSION_1_1)
		GRAPHICS_API extern PFN_vkBindBufferMemory2 vkBindBufferMemory2;
		GRAPHICS_API extern PFN_vkBindImageMemory2 vkBindImageMemory2;
		GRAPHICS_API extern PFN_vkCmdDispatchBase vkCmdDispatchBase;
		GRAPHICS_API extern PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
		GRAPHICS_API extern PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
		GRAPHICS_API extern PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
		GRAPHICS_API extern PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
		GRAPHICS_API extern PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
		GRAPHICS_API extern PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
		GRAPHICS_API extern PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups;
		GRAPHICS_API extern PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
		GRAPHICS_API extern PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
		GRAPHICS_API extern PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
		GRAPHICS_API extern PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
		GRAPHICS_API extern PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
		GRAPHICS_API extern PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2;
		GRAPHICS_API extern PFN_vkTrimCommandPool vkTrimCommandPool;
		GRAPHICS_API extern PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
#endif

#if defined(VK_AMD_buffer_marker)
		GRAPHICS_API extern PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif

#if defined(VK_AMD_draw_indirect_count)
		GRAPHICS_API extern PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
		GRAPHICS_API extern PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
#endif

#if defined(VK_AMD_shader_info)
		GRAPHICS_API extern PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif

#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
		GRAPHICS_API extern PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
		GRAPHICS_API extern PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif

#if defined(VK_ANDROID_native_buffer)
		GRAPHICS_API extern PFN_vkAcquireImageANDROID vkAcquireImageANDROID;
		GRAPHICS_API extern PFN_vkGetSwapchainGrallocUsageANDROID vkGetSwapchainGrallocUsageANDROID;
		GRAPHICS_API extern PFN_vkQueueSignalReleaseImageANDROID vkQueueSignalReleaseImageANDROID;
#endif

#if defined(VK_EXT_acquire_xlib_display)
		GRAPHICS_API extern PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT;
		GRAPHICS_API extern PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT;
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_conditional_rendering)
		GRAPHICS_API extern PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
		GRAPHICS_API extern PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
		GRAPHICS_API extern PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
		GRAPHICS_API extern PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
		GRAPHICS_API extern PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
		GRAPHICS_API extern PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
		GRAPHICS_API extern PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_debug_report)
		GRAPHICS_API extern PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
		GRAPHICS_API extern PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
		GRAPHICS_API extern PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
		GRAPHICS_API extern PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
		GRAPHICS_API extern PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
		GRAPHICS_API extern PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
		GRAPHICS_API extern PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
		GRAPHICS_API extern PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
		GRAPHICS_API extern PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
		GRAPHICS_API extern PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
		GRAPHICS_API extern PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
		GRAPHICS_API extern PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
		GRAPHICS_API extern PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
		GRAPHICS_API extern PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
		GRAPHICS_API extern PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT;
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_discard_rectangles)
		GRAPHICS_API extern PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_display_control)
		GRAPHICS_API extern PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
		GRAPHICS_API extern PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
		GRAPHICS_API extern PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
		GRAPHICS_API extern PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_display_surface_counter)
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_external_memory_host)
		GRAPHICS_API extern PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_hdr_metadata)
		GRAPHICS_API extern PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_sample_locations)
		GRAPHICS_API extern PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_validation_cache)
		GRAPHICS_API extern PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
		GRAPHICS_API extern PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
		GRAPHICS_API extern PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
		GRAPHICS_API extern PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_GOOGLE_display_timing)
		GRAPHICS_API extern PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
		GRAPHICS_API extern PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_KHR_android_surface)
		GRAPHICS_API extern PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_bind_memory2)
		GRAPHICS_API extern PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
		GRAPHICS_API extern PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_create_renderpass2)
		GRAPHICS_API extern PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
		GRAPHICS_API extern PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
		GRAPHICS_API extern PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
		GRAPHICS_API extern PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_descriptor_update_template)
		GRAPHICS_API extern PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
		GRAPHICS_API extern PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
		GRAPHICS_API extern PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
		GRAPHICS_API extern PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
		GRAPHICS_API extern PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
		GRAPHICS_API extern PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_device_group_creation)
		GRAPHICS_API extern PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR;
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
		GRAPHICS_API extern PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
		GRAPHICS_API extern PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
		GRAPHICS_API extern PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
		GRAPHICS_API extern PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
		GRAPHICS_API extern PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_display_swapchain)
		GRAPHICS_API extern PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
		GRAPHICS_API extern PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
		GRAPHICS_API extern PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_external_fence_capabilities)
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR;
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_fence_fd)
		GRAPHICS_API extern PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
		GRAPHICS_API extern PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
		GRAPHICS_API extern PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
		GRAPHICS_API extern PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_capabilities)
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_memory_fd)
		GRAPHICS_API extern PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
		GRAPHICS_API extern PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
		GRAPHICS_API extern PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
		GRAPHICS_API extern PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_capabilities)
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_external_semaphore_fd)
		GRAPHICS_API extern PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
		GRAPHICS_API extern PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
		GRAPHICS_API extern PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
		GRAPHICS_API extern PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_get_display_properties2)
		GRAPHICS_API extern PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR;
		GRAPHICS_API extern PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR;
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_memory_requirements2)
		GRAPHICS_API extern PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
		GRAPHICS_API extern PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
		GRAPHICS_API extern PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_get_physical_device_properties2)
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_maintenance1)
		GRAPHICS_API extern PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
		GRAPHICS_API extern PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_mir_surface)
		GRAPHICS_API extern PFN_vkCreateMirSurfaceKHR vkCreateMirSurfaceKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR;
#endif /* defined(VK_KHR_mir_surface) */
#if defined(VK_KHR_push_descriptor)
		GRAPHICS_API extern PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
		GRAPHICS_API extern PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
		GRAPHICS_API extern PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
		GRAPHICS_API extern PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_surface)
		GRAPHICS_API extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_swapchain)
		GRAPHICS_API extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
		GRAPHICS_API extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
		GRAPHICS_API extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
		GRAPHICS_API extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
		GRAPHICS_API extern PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_wayland_surface)
		GRAPHICS_API extern PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
		GRAPHICS_API extern PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
		GRAPHICS_API extern PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
		GRAPHICS_API extern PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
		GRAPHICS_API extern PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK;
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
		GRAPHICS_API extern PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
		GRAPHICS_API extern PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NVX_device_generated_commands)
		GRAPHICS_API extern PFN_vkCmdProcessCommandsNVX vkCmdProcessCommandsNVX;
		GRAPHICS_API extern PFN_vkCmdReserveSpaceForCommandsNVX vkCmdReserveSpaceForCommandsNVX;
		GRAPHICS_API extern PFN_vkCreateIndirectCommandsLayoutNVX vkCreateIndirectCommandsLayoutNVX;
		GRAPHICS_API extern PFN_vkCreateObjectTableNVX vkCreateObjectTableNVX;
		GRAPHICS_API extern PFN_vkDestroyIndirectCommandsLayoutNVX vkDestroyIndirectCommandsLayoutNVX;
		GRAPHICS_API extern PFN_vkDestroyObjectTableNVX vkDestroyObjectTableNVX;
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX;
		GRAPHICS_API extern PFN_vkRegisterObjectsNVX vkRegisterObjectsNVX;
		GRAPHICS_API extern PFN_vkUnregisterObjectsNVX vkUnregisterObjectsNVX;
#endif /* defined(VK_NVX_device_generated_commands) */
#if defined(VK_NV_clip_space_w_scaling)
		GRAPHICS_API extern PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_device_diagnostic_checkpoints)
		GRAPHICS_API extern PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
		GRAPHICS_API extern PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_external_memory_capabilities)
		GRAPHICS_API extern PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_external_memory_win32)
		GRAPHICS_API extern PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif /* defined(VK_NV_external_memory_win32) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1))
		GRAPHICS_API extern PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
		GRAPHICS_API extern PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
		GRAPHICS_API extern PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
		GRAPHICS_API extern PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
		GRAPHICS_API extern PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */

#ifdef __cplusplus
	}
#endif
