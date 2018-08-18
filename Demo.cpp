#include "Demo.h"

#include <iostream>
#include <fstream>
#include "files.h"
#include "RendererVK.h"
#include "BufferImpl.h"

namespace  {
    uint32_t width = 1600;
    uint32_t height = 990;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
    std::cout << "validation layer: " << msg << std::endl;
    
    return VK_FALSE;
}

VkResult CreateDebugReportCallbackEXT(const VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* createInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* callback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, createInfo, pAllocator, callback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

static std::vector<char> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    
    file.close();
    
    return buffer;
}

void DemoApplication::SetupDebugCallback()
{
    VkDebugReportCallbackCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags =
    VK_DEBUG_REPORT_WARNING_BIT_EXT |
    VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
    VK_DEBUG_REPORT_DEBUG_BIT_EXT |
    VK_DEBUG_REPORT_ERROR_BIT_EXT;
    createInfo.pfnCallback = debugCallback;
    
    CreateDebugReportCallbackEXT(VkInstance(instance), &createInfo, nullptr, &callback);
}

void DemoApplication::initVulkan()
{
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };
    
    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    
    mRenderer.Initialize();

    
    vk::ApplicationInfo appInfo;
    appInfo.pApplicationName = "Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
    };
    
    const std::vector<const char*> extensions =
    {
        "VK_EXT_debug_report",
        "VK_MVK_macos_surface",
        "VK_KHR_surface",
        "VK_MVK_macos_surface"
    };
    
    const std::vector<const char*> reqDeviceExtensions
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
//        "VK_MVK_moltenvk"
//        "VK_KHR_swapchain"
//        "VK_KHR_surface"
//        "VK_MVK_macos_surface"
    };
    

    
    vk::InstanceCreateInfo createInfo;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    
    try {
        instance = vk::createInstance(createInfo);
    } catch (vk::LayerNotPresentError e) {
        std::cout << e.what() << std::endl;
    }
    
    SetupDebugCallback();
    
    const auto devices = instance.enumeratePhysicalDevices();
    std::cout << "Available GPU devices: " << devices.size() << std::endl;
    
    for(const auto& device : devices)
    {
        const auto deviceProperties = device.getProperties();
        
        std::cout   << deviceProperties.deviceName
                    << ", Vulkan API version: "
                    << VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
                    << VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
                    << VK_VERSION_PATCH(deviceProperties.apiVersion) << std::endl;
    }
    
    mPhysicalDevice = PickDevice(devices);
    const auto queueProperties = mPhysicalDevice.getQueueFamilyProperties();
    
    std::cout << "Available queue families: " << queueProperties.size() << std::endl;
    for(const auto& queueFamily : queueProperties)
    {
        if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            std::cout << "G | ";
        if(queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
            std::cout << "C | ";
        if(queueFamily.queueFlags & vk::QueueFlagBits::eTransfer)
            std::cout << "T | ";
        if(queueFamily.queueFlags & vk::QueueFlagBits::eProtected)
            std::cout << "P | ";
        if(queueFamily.queueFlags & vk::QueueFlagBits::eSparseBinding)
            std::cout << "S | ";
        
        std::cout << "Queue count: " << queueFamily.queueCount << std::endl;
    }
    
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    
    std::vector<float> queuePriorities{ 1.0f };
    queueCreateInfos.push_back(detail::CreateQueues(0, queuePriorities));
    
    const auto deviceFeatures = mPhysicalDevice.getFeatures();
    
    vk::DeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(reqDeviceExtensions.size());;
    deviceCreateInfo.ppEnabledExtensionNames = reqDeviceExtensions.data();
    deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    
//    const auto deviceLayers = device.enumerateDeviceLayerProperties();
//    for(const auto& layer : deviceLayers)
//    {
//        std::cout << layer.layerName << ", v: " << layer.specVersion << std::endl;
//    }
//
    
    std::cout << "Device extensions: " << std::endl;
    const auto deviceExtensions = mPhysicalDevice.enumerateDeviceExtensionProperties();
    for(const auto& ext : deviceExtensions)
    {
        std::cout << ext.extensionName << ", v: " << VK_VERSION_MAJOR(ext.specVersion) << "."
        << VK_VERSION_MAJOR(ext.specVersion) << "."
        << VK_VERSION_PATCH(ext.specVersion) << std::endl;
    }
    
    try {
         mDevice = mPhysicalDevice.createDevice(deviceCreateInfo);
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
    
    mGraphicsQueue = mDevice.getQueue(0, 0);
    
    VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.pView = view;
    surfaceCreateInfo.flags = 0;
    
    auto CreateMacOSSurfaceKHR = (PFN_vkCreateMacOSSurfaceMVK) vkGetInstanceProcAddr(VkInstance(instance), "vkCreateMacOSSurfaceMVK");
    
    if (!CreateMacOSSurfaceKHR || CreateMacOSSurfaceKHR(VkInstance(instance), &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    
    if(1 /*device extension for swapchain is support*/)
    {
        QuerySwapChainProperties();
        
        const auto& format = mFormats.front();      // Get B8G8R8A8_unorm
        const auto& presentMode = mPresentationModes.front();   // Immeadiate
        vk::Extent2D screenExtent(::width, ::height);
        
        uint32_t imageCount = mCapabilities.minImageCount + 1;
        
        if(mEnableTrippleBuffering)
        {
            if(imageCount > mCapabilities.maxImageCount)
            {
                imageCount = mCapabilities.maxImageCount;
            }
        }
        
        vk::SwapchainCreateInfoKHR createInfo;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.setImageColorSpace(format.colorSpace);
        createInfo.setImageFormat(format.format);
        createInfo.setImageExtent(screenExtent);
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;       //Use transfer if I want to render to other buffer and apply post process and then copy;
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
        createInfo.preTransform = mCapabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = vk::SwapchainKHR{};
        
        mSwapChain = mDevice.createSwapchainKHR(createInfo);
        
        mSwapChainImages = mDevice.getSwapchainImagesKHR(mSwapChain);
        
        mSwapChainImageViews.resize(mSwapChainImages.size());
        for(size_t idx{ 0 }; idx < mSwapChainImageViews.size(); ++idx)
        {
            vk::ImageViewCreateInfo imageViewCreateInfo;
            imageViewCreateInfo.image = mSwapChainImages[idx];
            imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
            imageViewCreateInfo.format = format.format;
            imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;
            
            mSwapChainImageViews[idx] = mDevice.createImageView(imageViewCreateInfo);
        }
    }
    
    const auto vertSh = bundlePath("vert");
    const auto fragSh = bundlePath("frag");
    
    const auto vertShaderCode = readFile(vertSh);
    const auto fragShaderCode = readFile(fragSh);
    
    const auto mVertexShader = CreateShader(vertShaderCode);
    const auto mFragmentShader = CreateShader(fragShaderCode);
    
    vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = mVertexShader;
    vertShaderStageInfo.pName = "main";
    vertShaderStageInfo.pSpecializationInfo = nullptr;
    
    vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = mFragmentShader;
    fragShaderStageInfo.pName = "main";
    
    vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
    
    
    // Vertex input
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();
    
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    
    // Input assembler
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    
    //Viewport & scissor
    vk::Viewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) ::width;
    viewport.height = (float) ::height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    vk::Rect2D scissor;
    scissor.offset = vk::Offset2D{ 0, 0 };
    scissor.extent = vk::Extent2D(::width, ::height);
    
    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    
    // Rasterizer
    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
    
    // Multisampling
    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional
    
    // Blending
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne; // Optional
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero; // Optional
    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero; // Optional
    colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd; // Optional
    
    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional
    
    vk::DynamicState dynamicStates[] = { vk::DynamicState::eViewport };
    
    vk::PipelineDynamicStateCreateInfo dynamicState;
    dynamicState.dynamicStateCount = 1;
    dynamicState.pDynamicStates = dynamicStates;
    
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
    
    mPipelineLayout = mDevice.createPipelineLayout(pipelineLayoutInfo);
    
    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = mFormats.front().format;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
    
    vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
    
    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    
    
    vk::SubpassDependency dependency;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
    
    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    
    mRenderPass = mDevice.createRenderPass(renderPassInfo);
    
    vk::GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = mPipelineLayout;
    pipelineInfo.renderPass = mRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = vk::Pipeline{}; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional
    
    mGraphicsPipeline = mDevice.createGraphicsPipeline({}, pipelineInfo);
    
    mSwapChainFramebuffers.resize(mSwapChainImageViews.size());

    for (size_t i = 0; i < mSwapChainImageViews.size(); i++)
    {
        std::vector<vk::ImageView> attachments {
            mSwapChainImageViews[i]
        };
        
        vk::FramebufferCreateInfo framebufferInfo;
        framebufferInfo.renderPass = mRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = ::width;
        framebufferInfo.height = ::height;
        framebufferInfo.layers = 1;
        
        mSwapChainFramebuffers[i] = mDevice.createFramebuffer(framebufferInfo);
    }
    
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.queueFamilyIndex = 0;
    
    mCommandPool = mDevice.createCommandPool(poolInfo);
    
    //-------------------------------------------------
    using namespace renderer;
    
    BufferData vertexData;
    vertexData.data = (void*)vertices.data();
    vertexData.size = static_cast<uint32_t>(sizeof(vertices[0]) * vertices.size());
    
    mRenderer.SetDevice(mDevice, mPhysicalDevice, mCommandPool, mGraphicsQueue);
    
    mTriangleVBO = std::make_shared<Buffer>();
    mTriangleStagingVBO = std::make_shared<Buffer>();
    mTriangleIBO = std::make_shared<Buffer>();
    mTriangleStagingIBO = std::make_shared<Buffer>();
    
    BufferDesc stagingDesc;
    stagingDesc.usage = BufferUsage::Staging | BufferUsage::TransferSrc;
    
    mRenderer.CreateBuffer(stagingDesc, vertexData, mTriangleStagingVBO);
    
    auto stagingBufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleStagingVBO);
    const auto& stagingBufferMemory = stagingBufferVk->GetVulkanMemory();
    
    void* dataPtr = mDevice.mapMemory(stagingBufferMemory, 0, vertexData.size);
    memcpy(dataPtr, vertices.data(), (size_t)vertexData.size);
    mDevice.unmapMemory(stagingBufferMemory);
    
    BufferDesc desc;
    desc.flags = BufferBindFlags::VertexBuffer;
    desc.usage = BufferUsage::TransferDest;
    
    mRenderer.CreateBuffer(desc, vertexData, mTriangleVBO);
    
    auto bufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleVBO);
//    const auto& bufferMemory = bufferVk->GetVulkanMemory();
//    void* dataPtrr = mDevice.mapMemory(bufferMemory, 0, vertexData.size);
//    memcpy(dataPtrr, vertices.data(), (size_t)vertexData.size);
//    mDevice.unmapMemory(bufferMemory);
    
    mTriangleStagingVBO->CopyData(mTriangleStagingVBO, mTriangleVBO, 0, 0, vertexData.size);
    // vk Destroy staging buff
    // vk free staging buff memory
    
    BufferDesc stagingIboDesc;
    stagingIboDesc.usage = BufferUsage::Staging | BufferUsage::TransferSrc;
    
    BufferData indexData;
    indexData.data = (void*)indices.data();
    indexData.size = static_cast<uint32_t>(sizeof(indices[0]) * indices.size());
    
    mRenderer.CreateBuffer(stagingIboDesc, indexData, mTriangleStagingIBO);
    
    auto stagingIndexBufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleStagingIBO);
    const auto& stagingIndexBufferMemory = stagingIndexBufferVk->GetVulkanMemory();
    
    void* mdataPtr = mDevice.mapMemory(stagingIndexBufferMemory, 0, indexData.size);
    memcpy(mdataPtr, indices.data(), (size_t)indexData.size);
    mDevice.unmapMemory(stagingIndexBufferMemory);
    
    BufferDesc descIbo;
    descIbo.flags = BufferBindFlags::IndexBuffer;
    descIbo.usage = BufferUsage::TransferDest; // TODO: Device local
    
    mRenderer.CreateBuffer(descIbo, indexData, mTriangleIBO);
    
    mTriangleStagingIBO->CopyData(mTriangleStagingIBO, mTriangleIBO, 0, 0, indexData.size);
    
    auto indexBufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleIBO);
    
//   auto indexBufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleIBO);
//   const auto& indexBufferMemory = indexBufferVk->GetVulkanMemory();
    
//    void* mmdataPtr = mDevice.mapMemory(indexBufferMemory, 0, indexData.size);
//    memcpy(mmdataPtr, indices.data(), (size_t)indexData.size);
//    mDevice.unmapMemory(indexBufferMemory);
    
    //-------------------------------------------------
    
    mCommandBuffers.resize(mSwapChainFramebuffers.size());
    
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = mCommandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = (uint32_t)mCommandBuffers.size();
    
    mCommandBuffers = mDevice.allocateCommandBuffers(allocInfo);
    
    for (size_t i = 0; i < mCommandBuffers.size(); i++)
    {
        vk::CommandBufferBeginInfo beginInfo;
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
        beginInfo.pInheritanceInfo = nullptr; // Optional
        
        mCommandBuffers[i].begin(beginInfo);
        
        vk::RenderPassBeginInfo renderPassInfo;
        renderPassInfo.renderPass = mRenderPass;
        renderPassInfo.framebuffer = mSwapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
        renderPassInfo.renderArea.extent = vk::Extent2D(::width, ::height);
        vk::ClearColorValue clearColor(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        renderPassInfo.clearValueCount = 1;
        
        std::vector<vk::ClearValue> clearValue(1);
        clearValue[0].setColor(clearColor);
        renderPassInfo.pClearValues = clearValue.data();
        
        mCommandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
        mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, mGraphicsPipeline);
        
        std::vector<vk::Buffer> vertexBuffers{ bufferVk->GetVulkanBuffer() };
        std::vector<vk::DeviceSize> offsets{ 0 };
        mCommandBuffers[i].bindVertexBuffers(0, vertexBuffers, offsets);
        mCommandBuffers[i].bindIndexBuffer(indexBufferVk->GetVulkanBuffer(), 0, vk::IndexType::eUint16);
        mCommandBuffers[i].drawIndexed(indices.size(), 1, 0, 0, 0);
        mCommandBuffers[i].endRenderPass();
        mCommandBuffers[i].end();
    }
    
    mImageAvailableSemaphore = mDevice.createSemaphore({});
    mRenderFinishedSemaphore = mDevice.createSemaphore({});
}

const vk::PhysicalDevice& DemoApplication::PickDevice(const std::vector<vk::PhysicalDevice>& devices)
{
    return devices.back();
}

void DemoApplication::QuerySwapChainProperties()
{
    mCapabilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(surface);
    mFormats = mPhysicalDevice.getSurfaceFormatsKHR(surface);
    mPresentationModes = mPhysicalDevice.getSurfacePresentModesKHR(surface);
}

vk::ShaderModule DemoApplication::CreateShader(const std::vector<char>& sourceCode)
{
    vk::ShaderModuleCreateInfo createInfo;
    createInfo.codeSize = sourceCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());
    return mDevice.createShaderModule(createInfo);
}

vk::DeviceQueueCreateInfo detail::CreateQueues(const uint32_t familyIndex, const std::vector<float>& priorities)
{
    vk::DeviceQueueCreateInfo createInfo;
    createInfo.queueCount = static_cast<uint32_t>(priorities.size());
    createInfo.queueFamilyIndex = familyIndex;
    createInfo.pQueuePriorities = priorities.data();
    
    return createInfo;
}

void DemoApplication::cleanup()
{
    mDevice.waitIdle();
}

void DemoApplication::draw()
{    
    auto result = mDevice.acquireNextImageKHR(mSwapChain, std::numeric_limits<uint64_t>::max(), mImageAvailableSemaphore, {});
    uint32_t imageIndex = result.value;
    
    vk::SubmitInfo submitInfo;
    vk::Semaphore waitSemaphores[] = { mImageAvailableSemaphore };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &mCommandBuffers[imageIndex];
    
    vk::Semaphore signalSemaphores[] = { mRenderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
    std::vector<vk::SubmitInfo> data;
    data.push_back(submitInfo);
    
    mGraphicsQueue.submit(data, {});
    
    vk::PresentInfoKHR presentInfo;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = { mSwapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional
    mGraphicsQueue.presentKHR(presentInfo);
}
