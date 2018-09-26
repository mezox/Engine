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
#include "Effect.h"

namespace Renderer {
    class Buffer;
}

namespace Engine
{
	class IWindow;
}

class DemoApplication
{
public:
	RENDERER_API DemoApplication();
	RENDERER_API ~DemoApplication();

    void RENDERER_API cleanup();
    void RENDERER_API draw();
    void RENDERER_API initVulkan();

	RENDERER_API Engine::IWindow* GetWindow() { return mWindow.get(); }

public:
    void* view{ nullptr };

	std::unique_ptr<Engine::IWindow> mWindow;

private:
	VkDevice mDevice;
    VkRenderPass mRenderPass;
    VkPipelineLayout mPipelineLayout;
    VkPipeline mGraphicsPipeline;
	VkPipeline mGraphicsPipelineTex;
	VkPipeline mGraphicsPipelineMix;
	VkPipelineCache mPipelineCache;
    
    std::vector<VkCommandBuffer> mCommandBuffers;

	std::unique_ptr<Renderer::Effect> mEffect;
    
    VkSemaphore mImageAvailableSemaphore;
    VkSemaphore mRenderFinishedSemaphore;
    
    std::shared_ptr<Renderer::Buffer> mTriangleVBO;
    std::shared_ptr<Renderer::Buffer> mTriangleIBO;

	std::vector<std::shared_ptr<Renderer::Buffer>> mUniformBuffers;

	VkDescriptorPool mDescriptorPool;
	std::vector<VkDescriptorSet> mDescriptorSets;

	std::unique_ptr<Renderer::Texture> mTexture;

	glm::vec3 mCameraPosition;
    
    bool mEnableTrippleBuffering{ true };
};
