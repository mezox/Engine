#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <array>
#include <memory>

#include <vulkan/vulkan.hpp>

#include "RendererVK.h"

namespace detail
{
    vk::DeviceQueueCreateInfo CreateQueues(const uint32_t familyIndex, const std::vector<float>& priorities);
}

namespace renderer {
    class IBuffer;
}

class DemoApplication
{
public:
    void cleanup();
    void draw();
    void initVulkan();
    
private:
    void SetupDebugCallback();
    
    const vk::PhysicalDevice& PickDevice(const std::vector<vk::PhysicalDevice>& devices);
    void QuerySwapChainProperties();
    
    vk::ShaderModule CreateShader(const std::vector<char>& sourceCode);
    
public:
    void* view{ nullptr };
    
private:
    vk::Instance instance;
    VkDebugReportCallbackEXT callback;
    vk::PhysicalDevice mPhysicalDevice;
    vk::Device mDevice;
    vk::Queue mGraphicsQueue;
    VkSurfaceKHR surface;
    
    // Swap chain properties
    vk::SurfaceCapabilitiesKHR mCapabilities;
    std::vector<vk::SurfaceFormatKHR> mFormats;
    std::vector<vk::PresentModeKHR> mPresentationModes;
    
    vk::SwapchainKHR mSwapChain;
    std::vector<vk::Image> mSwapChainImages;
    std::vector<vk::ImageView> mSwapChainImageViews;
    
    vk::RenderPass mRenderPass;
    vk::PipelineLayout mPipelineLayout;
    vk::Pipeline mGraphicsPipeline;
    
    std::vector<vk::Framebuffer> mSwapChainFramebuffers;
    
    vk::CommandPool mCommandPool;
    std::vector<vk::CommandBuffer> mCommandBuffers;
    
    vk::Semaphore mImageAvailableSemaphore;
    vk::Semaphore mRenderFinishedSemaphore;
    
    renderer::RendererVK mRenderer;
    
    std::shared_ptr<renderer::IBuffer> mTriangleVBO;
    std::shared_ptr<renderer::IBuffer> mTriangleStagingVBO;
    std::shared_ptr<renderer::IBuffer> mTriangleStagingIBO;
    std::shared_ptr<renderer::IBuffer> mTriangleIBO;
    
    bool mEnableTrippleBuffering{ true };
};
