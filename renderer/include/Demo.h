#pragma once

#include "RendererBase.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <array>
#include <memory>

#include <PAL/Graphics/LowVK.h>

#include "RendererVK.h"
#include "Texture.h"

#ifdef WIN32
#include <GLFW/glfw3.h>
#endif

namespace renderer {
    class IBuffer;
}

class DemoApplication
{
public:
    void RENDERER_API cleanup();
    void RENDERER_API draw();
    void RENDERER_API initVulkan();
    
private:
    
public:
    void* view{ nullptr };
    

#ifdef WIN32
	GLFWwindow* window;
#endif

private:
	VkDevice mDevice;
    VkRenderPass mRenderPass;
    VkPipelineLayout mPipelineLayout;
    VkPipeline mGraphicsPipeline;
    
    std::vector<VkFramebuffer> mSwapChainFramebuffers;
    
    std::vector<VkCommandBuffer> mCommandBuffers;
    
    VkSemaphore mImageAvailableSemaphore;
    VkSemaphore mRenderFinishedSemaphore;
    
    renderer::RendererVK mRenderer;
    
    std::shared_ptr<renderer::IBuffer> mTriangleVBO;
    std::shared_ptr<renderer::IBuffer> mTriangleIBO;

	std::vector<std::shared_ptr<renderer::IBuffer>> mUniformBuffers;

	VkDescriptorPool mDescriptorPool;
	std::vector<VkDescriptorSet> mDescriptorSets;

	std::unique_ptr<renderer::Texture> mTexture;
	std::unique_ptr<renderer::Texture> mDepthTexture;

	glm::vec3 mCameraPosition;
    
    bool mEnableTrippleBuffering{ true };
};
