#pragma once

#include "RendererBase.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <array>
#include <memory>

#include <vulkan/vulkan.hpp>

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
    void QuerySwapChainProperties();
    
    vk::ShaderModule CreateShader(const std::vector<char>& sourceCode);
    
public:
    void* view{ nullptr };
    

#ifdef WIN32
	GLFWwindow* window;
#endif

private:
	vk::Device mDevice;
    
    vk::RenderPass mRenderPass;
    vk::PipelineLayout mPipelineLayout;
    vk::Pipeline mGraphicsPipeline;
    
    std::vector<vk::Framebuffer> mSwapChainFramebuffers;
    
    std::vector<vk::CommandBuffer> mCommandBuffers;
    
    vk::Semaphore mImageAvailableSemaphore;
    vk::Semaphore mRenderFinishedSemaphore;
    
    renderer::RendererVK mRenderer;
    
    std::shared_ptr<renderer::IBuffer> mTriangleVBO;
    std::shared_ptr<renderer::IBuffer> mTriangleIBO;

	std::vector<std::shared_ptr<renderer::IBuffer>> mUniformBuffers;

	vk::DescriptorPool mDescriptorPool;
	std::vector<vk::DescriptorSet> mDescriptorSets;

	renderer::Texture mTexture;
	renderer::Texture mDepthTexture;

	glm::vec3 mCameraPosition;

	//vk::Image mDepthBufferImage;
	//vk::DeviceMemory mDepthBufferImageMemory;
	//vk::ImageView mDepthImageView;
    
    bool mEnableTrippleBuffering{ true };
};
