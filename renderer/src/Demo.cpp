#include "Demo.h"

#include <iostream>
#include <fstream>
#include "files.h"
#include "RendererVK.h"
#include "BufferImpl.h"
#include "Texture.h"
#include "Object3D.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE			// for depth in range 0,1 instead of -1,1
#include <glm/gtc/matrix_transform.hpp>

#ifdef WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>
#include <chrono>

#undef min
#undef max
#endif

namespace  {
    uint32_t width = 1600;
    uint32_t height = 990;
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

void RENDERER_API DemoApplication::initVulkan()
{
#ifdef WIN32
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(1920, 1080, "Vulkan", nullptr, nullptr);
#endif
    
    mRenderer.Initialize(window);

	//Renderer::Cube cube;
	Renderer::Mesh cube;
	cube.Load("resources/models/chalet.obj");

	mDevice = mRenderer.GetDevice();

	mCameraPosition = glm::vec3(2.0f, 2.0f, 2.0f);
    

    
    
#ifdef WIN32
	const auto vertSh = std::string("resources/shaders/vert.spv");
	const auto fragSh = std::string("resources/shaders/frag.spv");
#else
	const auto vertSh = bundlePath("vert");
	const auto fragSh = bundlePath("frag");
#endif

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
    rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
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

	// -------- Create uniform buffer -----------------

	vk::DeviceSize bufferSize = sizeof(CameraTransform);

	mUniformBuffers.resize(/*mSwapChainImages.size()*/3);

	for (size_t i = 0; i < /*mSwapChainImages.size()*/3; ++i)
	{
		renderer::BufferDesc desc(renderer::BufferUsage::Staging, renderer::BufferBindFlags::UniformBuffer);
		renderer::BufferData data(nullptr, static_cast<uint32_t>(sizeof(CameraTransform)));

		mRenderer.CreateBuffer(desc, data, mUniformBuffers[i]);
		//TODO: Map
	}

	auto renderer = std::make_shared<renderer::RendererVK>(mRenderer);

	// ------------------------------------------------
	mTexture = renderer::Texture(renderer, "resources/textures/chalet.jpg");
	// -------------------------------------------------

	mRenderer.CreateDescriptorSetLayout();
	const auto& setLayout = mRenderer.GetDescriptorSetLayout();
	const auto setLayoutCopy = setLayout;

	std::array<vk::DescriptorPoolSize, 2> poolSizes;
	poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(/*mSwapChainImages.size()*/3);
	poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(/*mSwapChainImages.size()*/3);

	vk::DescriptorPoolCreateInfo descPoolInfo;
	descPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	descPoolInfo.pPoolSizes = poolSizes.data();
	descPoolInfo.maxSets = static_cast<uint32_t>(/*mSwapChainImages.size()*/3);

	mDescriptorPool = mDevice.createDescriptorPool(descPoolInfo);

	std::vector<vk::DescriptorSetLayout> layouts(/*mSwapChainImages.size()*/3, setLayoutCopy);

	vk::DescriptorSetAllocateInfo descAllocInfo;
	descAllocInfo.descriptorPool = mDescriptorPool;
	descAllocInfo.descriptorSetCount = static_cast<uint32_t>(/*mSwapChainImages.size()*/3);
	descAllocInfo.pSetLayouts = layouts.data();

	mDescriptorSets = mDevice.allocateDescriptorSets(descAllocInfo);

	for (size_t i = 0; i < /*mSwapChainImages.size()*/3; i++)
	{
		vk::DescriptorBufferInfo bufferInfo;
		bufferInfo.buffer = static_cast<renderer::Buffer*>(mUniformBuffers[i].get())->GetVulkanBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(CameraTransform);

		renderer::RendererTextureVK* vulkanTexture = (renderer::RendererTextureVK*)mTexture.GetRendererTexture();

		vk::DescriptorImageInfo imageInfo;
		imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		imageInfo.imageView = vulkanTexture->imageView;
		imageInfo.sampler = vulkanTexture->sampler;


		std::array<vk::WriteDescriptorSet, 2> descriptorWrites;
		descriptorWrites[0].dstSet = mDescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pImageInfo = nullptr; // Optional
		descriptorWrites[0].pTexelBufferView = nullptr; // Optional

		descriptorWrites[1].dstSet = mDescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		mDevice.updateDescriptorSets(descriptorWrites, {});
	}
    
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &setLayout;
    
    mPipelineLayout = mDevice.createPipelineLayout(pipelineLayoutInfo);
    
    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = mRenderer.GetFormats().front().format;
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
    
	// depth buffer
	vk::AttachmentDescription depthAttachment;
	depthAttachment.format = vk::Format::eD24UnormS8Uint;
	depthAttachment.samples = vk::SampleCountFlagBits::e1;
	depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
	depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::AttachmentReference depthAttachmentRef;
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef; 
    
    vk::SubpassDependency dependency;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

	std::array<vk::AttachmentDescription, 2> attachments{ colorAttachment, depthAttachment };
    
    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    
    mRenderPass = mDevice.createRenderPass(renderPassInfo);

	vk::PipelineDepthStencilStateCreateInfo depthStencil;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = vk::CompareOp::eLess;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional
    
    vk::GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState; // Optional
    pipelineInfo.layout = mPipelineLayout;
    pipelineInfo.renderPass = mRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = vk::Pipeline{}; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optionalpipe
    
    mGraphicsPipeline = mDevice.createGraphicsPipeline({}, pipelineInfo);
    
    mSwapChainFramebuffers.resize(/*mSwapChainImages.size()*/3);

	mDepthTexture = renderer::Texture(renderer, renderer::ImageFormat::DEPTH, renderer::ImageUsage::DepthAttachment, ::width, ::height);

    for (size_t i = 0; i < /*mSwapChainImages.size()*/3; i++)
    {
		auto depthTex = (renderer::RendererTextureVK*)mDepthTexture.GetRendererTexture();

        std::vector<vk::ImageView> attachments {
            mRenderer.GetSwapImageViews()[i],
			depthTex->imageView
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
    
    //-------------------------------------------------
    using namespace renderer;
    
    BufferData vertexData;
    vertexData.data = (void*)cube.vertices.data();
    vertexData.size = static_cast<uint32_t>(sizeof(cube.vertices[0]) * cube.vertices.size());
    
    
    mTriangleVBO = std::make_shared<Buffer>();
	std::shared_ptr<renderer::IBuffer> mTriangleStagingVBO = std::make_shared<Buffer>();
    mTriangleIBO = std::make_shared<Buffer>();
	std::shared_ptr<renderer::IBuffer> mTriangleStagingIBO = std::make_shared<Buffer>();
    
    BufferDesc stagingDesc;
    stagingDesc.usage = BufferUsage::Staging | BufferUsage::TransferSrc;
    
    mRenderer.CreateBuffer(stagingDesc, vertexData, mTriangleStagingVBO);
    
    auto stagingBufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleStagingVBO);
    const auto& stagingBufferMemory = stagingBufferVk->GetVulkanMemory();
    
    void* dataPtr = mDevice.mapMemory(stagingBufferMemory, 0, vertexData.size);
    memcpy(dataPtr, cube.vertices.data(), (size_t)vertexData.size);
    mDevice.unmapMemory(stagingBufferMemory);
    
    BufferDesc desc;
    desc.flags = BufferBindFlags::VertexBuffer;
    desc.usage = BufferUsage::TransferDest;
    
    mRenderer.CreateBuffer(desc, vertexData, mTriangleVBO);
    
    auto bufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleVBO);
    
    mTriangleStagingVBO->CopyData(mTriangleStagingVBO, mTriangleVBO, 0, 0, vertexData.size);
	mTriangleStagingVBO->Release();
	mTriangleStagingVBO.reset();
    // vk Destroy staging buff
    // vk free staging buff memory
    
    BufferDesc stagingIboDesc;
    stagingIboDesc.usage = BufferUsage::Staging | BufferUsage::TransferSrc;
    
    BufferData indexData;
    indexData.data = (void*)cube.indices.data();
    indexData.size = static_cast<uint32_t>(sizeof(cube.indices[0]) * cube.indices.size());
    
    mRenderer.CreateBuffer(stagingIboDesc, indexData, mTriangleStagingIBO);
    
    auto stagingIndexBufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleStagingIBO);
    const auto& stagingIndexBufferMemory = stagingIndexBufferVk->GetVulkanMemory();
    
    void* mdataPtr = mDevice.mapMemory(stagingIndexBufferMemory, 0, indexData.size);
    memcpy(mdataPtr, cube.indices.data(), (size_t)indexData.size);
    mDevice.unmapMemory(stagingIndexBufferMemory);
    
    BufferDesc descIbo;
    descIbo.flags = BufferBindFlags::IndexBuffer;
    descIbo.usage = BufferUsage::TransferDest;
    
    mRenderer.CreateBuffer(descIbo, indexData, mTriangleIBO);
    
    mTriangleStagingIBO->CopyData(mTriangleStagingIBO, mTriangleIBO, 0, 0, indexData.size);
    
    auto indexBufferVk = std::static_pointer_cast<renderer::Buffer>(mTriangleIBO);
	mTriangleStagingIBO->Release();
	mTriangleStagingIBO.reset();
    
    //-------------------------------------------------
    
    mCommandBuffers.resize(mSwapChainFramebuffers.size());
    
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = mRenderer.GetPool();
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

        vk::ClearColorValue clearColor(std::array<float, 4>{0.0f, 0.5f, 0.8f, 1.0f});
		vk::ClearDepthStencilValue clearColorDepth(1.0f, 0);

		std::vector<vk::ClearValue> clearValue(2);
		clearValue[0].setColor(clearColor);
		clearValue[1].setDepthStencil(clearColorDepth);

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValue.size());
        renderPassInfo.pClearValues = clearValue.data();

		vk::Viewport vp(0.0f, 0.0f, ::width, ::height, 0.0f, 1.0f);
        
        mCommandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		mCommandBuffers[i].setViewport(0, vp);
        
        std::vector<vk::Buffer> vertexBuffers{ bufferVk->GetVulkanBuffer() };
        std::vector<vk::DeviceSize> offsets{ 0 };
        mCommandBuffers[i].bindVertexBuffers(0, vertexBuffers, offsets);
        mCommandBuffers[i].bindIndexBuffer(indexBufferVk->GetVulkanBuffer(), 0, vk::IndexType::eUint32);
		mCommandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout, 0, 1, &mDescriptorSets[i], 0, nullptr);
        mCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, mGraphicsPipeline);
 
		vp.width = ::width / 2.0f;
		mCommandBuffers[i].setViewport(0, vp);
		mCommandBuffers[i].drawIndexed(static_cast<uint32_t>(cube.indices.size()), 1, 0, 0, 0);

		vp.x = ::width / 2.0f;
		mCommandBuffers[i].setViewport(0, vp);
		mCommandBuffers[i].drawIndexed(static_cast<uint32_t>(cube.indices.size()), 1, 0, 0, 0);

        mCommandBuffers[i].endRenderPass();
        mCommandBuffers[i].end();
    }
    
    mImageAvailableSemaphore = mDevice.createSemaphore({});
    mRenderFinishedSemaphore = mDevice.createSemaphore({});

	cube.vertices.clear();
	cube.indices.clear();
}

vk::ShaderModule DemoApplication::CreateShader(const std::vector<char>& sourceCode)
{
    vk::ShaderModuleCreateInfo createInfo;
    createInfo.codeSize = sourceCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());
    return mDevice.createShaderModule(createInfo);
}

void DemoApplication::cleanup()
{
	mDevice.waitIdle();

	for (auto& framebuffer : mSwapChainFramebuffers)
	{
		mDevice.destroyFramebuffer(framebuffer);
	}

	mDevice.freeCommandBuffers(mRenderer.GetPool(), mCommandBuffers);
	mDevice.destroyPipeline(mGraphicsPipeline);
	mDevice.destroyPipelineLayout(mPipelineLayout);
	mDevice.destroyRenderPass(mRenderPass);

	mRenderer.Deinitialize();
}

void RENDERER_API DemoApplication::draw()
{    
    auto result = mDevice.acquireNextImageKHR(mRenderer.GetSwapChain(), std::numeric_limits<uint64_t>::max(), mImageAvailableSemaphore, {});
    uint32_t imageIndex = result.value;

	// ------- Update Uniform buffer object ----------
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	//mCameraPosition.z += time * 0.001f;

	CameraTransform transform;
	transform.model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	transform.model = glm::rotate(transform.model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transform.view = glm::lookAt(mCameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform.projection = glm::perspective(glm::radians(60.0f), (float)::width / (float)::height, 0.1f, 10.0f);
	
	transform.projection[1][1] *= -1;

	//mUniformBuffers[imageIndex]->Map(0, sizeof(CameraTransform), &transform);

	const auto vkunibuff = static_cast<renderer::Buffer*>(mUniformBuffers[imageIndex].get());

	void* dataPtr = mDevice.mapMemory(vkunibuff->GetVulkanMemory(), 0, vk::DeviceSize(sizeof(CameraTransform)));
	memcpy(dataPtr, &transform, (size_t)sizeof(CameraTransform));
	mDevice.unmapMemory(vkunibuff->GetVulkanMemory());


	// ----------------- end of update ubo ------------
    
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
    
    mRenderer.GetGraphicsQueue().submit(data, {});
    
    vk::PresentInfoKHR presentInfo;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = { mRenderer.GetSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional
	mRenderer.GetGraphicsQueue().presentKHR(presentInfo);
}
