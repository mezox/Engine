#include "Demo.h"

#include "Buffer.h"
#include "Texture.h"
#include "Object3D.h"
#include "RendererResourceStateVK.h"
#include "Window.h"
#include "SwapChainImpl.h"
#include "Engine.h"
#include "RenderAPIImpl.h"
#include <PAL/Graphics/LowVK.h>
#include <PAL/FileSystem/LowFileSystem.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE			// for depth in range 0,1 instead of -1,1
#include <glm/gtc/matrix_transform.hpp>

#ifdef WIN32
#include <vulkan/vulkan_win32.h>
#include <chrono>
#endif

using namespace Engine;
using namespace PAL::FileSystem;

namespace 
{
    uint32_t width = 1920;
    uint32_t height = 1080;
}

void RENDERER_API DemoApplication::initVulkan()
{
    EngineServiceLocator::Provide(CreateEngineService());
    EngineServiceLocator::Service()->Initialize();
    mWindow = EngineServiceLocator::Service()->CreateWindow();

    auto* renderer = (Engine::VulkanRenderAPI*)Engine::RenderAPIServiceLocator::Service().get();
    auto& mRenderer = *renderer;
    mRenderer.Initialize(mWindow->GetNativeHandle() ? mWindow->GetNativeHandle() : view);
    
    mWindow->CreateSwapChain();

	Renderer::Cube cube;
	/*Renderer::Mesh cube;
	cube.Load("resources/models/chalet.obj");
*/
    mDevice = mRenderer.GetDevice();

	mCameraPosition = glm::vec3(0.0f, 1.0f, 3.0f);
    
    //const auto vertSh = FileSystemServiceLocator::Service()->GetFilePath("vert.spv");
    //const auto fragSh = FileSystemServiceLocator::Service()->GetFilePath("frag.spv");

	mEffect = std::make_unique<Renderer::Effect>();
	mEffect->Build("vert.spv", "frag.spv");
   

	// -------- Create uniform buffer -----------------
	mUniformBuffers.resize(/*mSwapChainImages.size()*/3);

	for (size_t i = 0; i < /*mSwapChainImages.size()*/3; ++i)
	{
		Renderer::BufferDesc desc(Renderer::BufferUsage::Staging, Renderer::BufferBindFlags::UniformBuffer);
		Renderer::BufferData data(nullptr, static_cast<uint32_t>(sizeof(CameraTransform)));

		mRenderer.CreateBuffer(desc, data, mUniformBuffers[i]);
	}

    const auto texPath = FileSystemServiceLocator::Service()->GetFilePath("chalet.jpg");
    mTexture = std::make_unique<Renderer::Texture>(texPath);

	mRenderer.CreateDescriptorSetLayout();
	const auto setLayout = mRenderer.GetDescriptorSetLayout();

	std::array<VkDescriptorPoolSize, 2> poolSizes;
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(/*mSwapChainImages.size()*/3);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(/*mSwapChainImages.size()*/3);

	VkDescriptorPoolCreateInfo descPoolInfo{};
	descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	descPoolInfo.pPoolSizes = poolSizes.data();
	descPoolInfo.maxSets = static_cast<uint32_t>(/*mSwapChainImages.size()*/3);

	LowVK::CreateDescriptorPool(&descPoolInfo, nullptr, &mDescriptorPool);

	std::vector<VkDescriptorSetLayout> layouts(/*mSwapChainImages.size()*/3, setLayout);

	VkDescriptorSetAllocateInfo descAllocInfo{};
	descAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descAllocInfo.descriptorPool = mDescriptorPool;
	descAllocInfo.descriptorSetCount = static_cast<uint32_t>(/*mSwapChainImages.size()*/3);
	descAllocInfo.pSetLayouts = layouts.data();

	mDescriptorSets.resize(3);
	LowVK::AllocateDescriptorSets(&descAllocInfo, mDescriptorSets.data());

	for (size_t i = 0; i < /*mSwapChainImages.size()*/3; i++)
	{
		auto bufferVk = (Renderer::BufferVK*)(mUniformBuffers[i]->GetGpuResource());

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = bufferVk->buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(CameraTransform);

		Renderer::RendererTextureVK* vulkanTexture = (Renderer::RendererTextureVK*)mTexture->GetRendererTexture();

		VkDescriptorImageInfo imageInfo;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = vulkanTexture->imageView;
		imageInfo.sampler = vulkanTexture->sampler;

		std::vector<VkWriteDescriptorSet> descriptorWrites(2);
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = mDescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pImageInfo = nullptr; // Optional
		descriptorWrites[0].pTexelBufferView = nullptr; // Optional

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = mDescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		LowVK::UpdateDescriptorSets(descriptorWrites, {});
	}
    
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &setLayout;
    
    LowVK::CreatePipelineLayout(&pipelineLayoutInfo, nullptr, &mPipelineLayout);
    
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    VkAttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
	// depth buffer
	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = VK_FORMAT_D24_UNORM_S8_UINT;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef;
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef; 
    
	VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments{ colorAttachment, depthAttachment };
    
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    
    LowVK::CreateRenderPass(&renderPassInfo, nullptr, &mRenderPass);

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	auto vertexShaderResource = (Renderer::EffectVK*)mEffect->mVertexShader.get();
	auto fragmentShaderResource = (Renderer::EffectVK*)mEffect->mFragmentShader.get();

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertexShaderResource->module;
	vertShaderStageInfo.pName = "main";
	vertShaderStageInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragmentShaderResource->module;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[]{ vertShaderStageInfo, fragShaderStageInfo };
    
	// Vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// Input assembler
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//Viewport & scissor
	VkViewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) ::width;
	viewport.height = (float) ::height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	scissor.offset = VkOffset2D{ 0, 0 };
	scissor.extent.width = ::width;
	scissor.extent.height = ::height;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional


	// Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	// Blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT };

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 1;
	dynamicState.pDynamicStates = dynamicStates;

	uint32_t colorMode{ 0 }; // Sets the lighting model used in the fragment "uber" shader

	VkSpecializationMapEntry specializationMapEntries;
	specializationMapEntries.constantID = 0;
	specializationMapEntries.size = sizeof(uint32_t);
	specializationMapEntries.offset = 0;

	// Prepare specialization info block for the shader stage
	VkSpecializationInfo specializationInfo{};
	specializationInfo.dataSize = sizeof(uint32_t);
	specializationInfo.mapEntryCount = 1;
	specializationInfo.pMapEntries = &specializationMapEntries;
	specializationInfo.pData = &colorMode;

	shaderStages[1].pSpecializationInfo = &specializationInfo; 
    
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
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
    pipelineInfo.basePipelineHandle = VkPipeline{}; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optionalpipe
    
	LowVK::CreateGraphicsPipelines({}, { pipelineInfo }, nullptr, &mGraphicsPipeline);

	colorMode = 1;
	LowVK::CreateGraphicsPipelines({}, { pipelineInfo }, nullptr, &mGraphicsPipelineTex);

	colorMode = 2;
	LowVK::CreateGraphicsPipelines({}, { pipelineInfo }, nullptr, &mGraphicsPipelineMix);
    
    //-------------------------------------------------
    
    Renderer::BufferData vertexData;
    vertexData.data = (void*)cube.vertices.data();
    vertexData.size = static_cast<uint32_t>(sizeof(cube.vertices[0]) * cube.vertices.size());
    
    
    mTriangleVBO = std::make_shared<Renderer::Buffer>();
	std::shared_ptr<Renderer::Buffer> mTriangleStagingVBO = std::make_shared<Renderer::Buffer>();
    mTriangleIBO = std::make_shared<Renderer::Buffer>();
	std::shared_ptr<Renderer::Buffer> mTriangleStagingIBO = std::make_shared<Renderer::Buffer>();
    
	Renderer::BufferDesc stagingDesc;
    stagingDesc.usage = Renderer::BufferUsage::Staging | Renderer::BufferUsage::TransferSrc;
    
    mRenderer.CreateBuffer(stagingDesc, vertexData, mTriangleStagingVBO);
    
    auto stagingBufferVk = (Renderer::BufferVK*)(mTriangleStagingVBO->GetGpuResource());
    
	void* dataPtr{ nullptr };
	LowVK::MapMemory(stagingBufferVk->memory, 0, vertexData.size, 0, &dataPtr);
    memcpy(dataPtr, cube.vertices.data(), (size_t)vertexData.size);
	LowVK::UnmapMemory(stagingBufferVk->memory);
    
	Renderer::BufferDesc desc;
    desc.flags = Renderer::BufferBindFlags::VertexBuffer;
    desc.usage = Renderer::BufferUsage::TransferDest;
    
    mRenderer.CreateBuffer(desc, vertexData, mTriangleVBO);
    
    auto bufferVk = (Renderer::BufferVK*)(mTriangleVBO->GetGpuResource());
    
    mRenderer.CopyBuffer(mTriangleStagingVBO, mTriangleVBO, 0, 0, vertexData.size);
	//mTriangleStagingVBO->Release();
	//mTriangleStagingVBO.reset();
    // vk Destroy staging buff
    // vk free staging buff memory
    
	Renderer::BufferDesc stagingIboDesc;
    stagingIboDesc.usage = Renderer::BufferUsage::Staging | Renderer::BufferUsage::TransferSrc;
    
	Renderer::BufferData indexData;
    indexData.data = (void*)cube.indices.data();
    indexData.size = static_cast<uint32_t>(sizeof(cube.indices[0]) * cube.indices.size());
    
    mRenderer.CreateBuffer(stagingIboDesc, indexData, mTriangleStagingIBO);
    
    auto stagingIndexBufferVk = (Renderer::BufferVK*)(mTriangleStagingIBO->GetGpuResource());
	const auto& stagingIndexBufferMemory = stagingIndexBufferVk->memory;
    
	void* mdataPtr{ nullptr };
	LowVK::MapMemory(stagingIndexBufferMemory, 0, indexData.size, 0, &mdataPtr);
    memcpy(mdataPtr, cube.indices.data(), (size_t)indexData.size);
    LowVK::UnmapMemory(stagingIndexBufferMemory);
    
	Renderer::BufferDesc descIbo;
    descIbo.flags = Renderer::BufferBindFlags::IndexBuffer;
    descIbo.usage = Renderer::BufferUsage::TransferDest;
    
    mRenderer.CreateBuffer(descIbo, indexData, mTriangleIBO);
    
    mRenderer.CopyBuffer(mTriangleStagingIBO, mTriangleIBO, 0, 0, indexData.size);
    
    auto indexBufferVk = (Renderer::BufferVK*)(mTriangleIBO->GetGpuResource());
	//mTriangleStagingIBO->Release();
	//mTriangleStagingIBO.reset();
    
   //-------------------------------------------------
    
    mCommandBuffers.resize(mWindow->GetSwapChain()->GetImageCount());
    
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mRenderer.GetPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)mCommandBuffers.size();
    
    LowVK::AllocateCommandBuffers(&allocInfo, mCommandBuffers.data());

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	LowVK::CreateSemaphores(&semaphoreInfo, nullptr, &mImageAvailableSemaphore);
	LowVK::CreateSemaphores(&semaphoreInfo, nullptr, &mRenderFinishedSemaphore);

    auto swapChain = (Renderer::SwapChainVK*)mWindow->GetSwapChain();
	swapChain->SetSemaphore(::width, ::height, mRenderer.GetGraphicsQueue(), mRenderFinishedSemaphore, mImageAvailableSemaphore, mRenderPass);
    
    const auto& swapChainFramebuffers = swapChain->GetFramebuffers();
    
	for (size_t i = 0; i < mCommandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		LowVK::BeginCommandBuffer(mCommandBuffers[i], &beginInfo);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mRenderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = VkOffset2D{ 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D{ ::width, ::height };

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		std::vector<VkBuffer> vertexBuffers{ bufferVk->buffer };
		std::vector<VkDeviceSize> offsets{ 0 };

		LowVK::CmdBeginRenderPass(mCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport vp{ 0.0f, 0.0f, (float)::width, (float)::height, 0.0f, 1.0f };

		LowVK::CmdSetViewport(mCommandBuffers[i], 0, { vp });
		LowVK::CmdBindDescriptorSets(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, { mDescriptorSets[i] }, {});
		LowVK::CmdBindVertexBuffers(mCommandBuffers[i], 0, 1, vertexBuffers.data(), offsets.data());
		LowVK::CmdBindIndexBuffer(mCommandBuffers[i], indexBufferVk->buffer, 0, VK_INDEX_TYPE_UINT32);

		// Left
		vp.width = (float)vp.width / 3.0f;

		LowVK::CmdSetViewport(mCommandBuffers[i], 0, { vp });
		LowVK::CmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);
		LowVK::CmdDrawIndexed(mCommandBuffers[i], static_cast<uint32_t>(cube.indices.size()), 1, 0, 0, 0);
		
		vp.x = ::width / 3.0f;
		LowVK::CmdSetViewport(mCommandBuffers[i], 0, { vp });
		LowVK::CmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipelineTex);
		LowVK::CmdDrawIndexed(mCommandBuffers[i], static_cast<uint32_t>(cube.indices.size()), 1, 0, 0, 0);

		vp.x = (::width / 3.0f) * 2.0f;
		LowVK::CmdSetViewport(mCommandBuffers[i], 0, { vp });
		LowVK::CmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipelineMix);
		LowVK::CmdDrawIndexed(mCommandBuffers[i], static_cast<uint32_t>(cube.indices.size()), 1, 0, 0, 0);

		LowVK::CmdEndRenderPass(mCommandBuffers[i]);
		LowVK::EndCommandBuffer(mCommandBuffers[i]);
	}

	cube.vertices.clear();
	cube.indices.clear();

	LowVK::DestroyShaderModule(vertexShaderResource->module, nullptr);
	LowVK::DestroyShaderModule(fragmentShaderResource->module, nullptr);
}

DemoApplication::DemoApplication()
{
    std::cout << "Created Demo Application" << std::endl;
}

DemoApplication::~DemoApplication()
{
    std::cout << "Destroyed Demo Application" << std::endl;
}

void DemoApplication::cleanup()
{
    auto* renderer = (Engine::VulkanRenderAPI*)Engine::RenderAPIServiceLocator::Service().get();
    auto& mRenderer = *renderer;
    
	LowVK::DestroyDescriptorPool(mDescriptorPool, nullptr);

	mTexture->ReleaseFromServer();

	LowVK::DestroyDescriptorPool(mDescriptorPool, nullptr);

	for (auto& uniformBuf : mUniformBuffers)
	{
		//uniformBuf->Release();
	}

	LowVK::FreeCommandBuffers(mRenderer.GetPool(), mCommandBuffers);
	//LowVK::DestroyPipeline(mGraphicsPipeline, nullptr);
	//vkDestroyPipelineLayout(mDevice, mPipelineLayout, nullptr);
	//vkDestroyRenderPass(mDevice, mRenderPass, nullptr);

	mRenderer.Deinitialize();
    
    Engine::RenderAPIServiceLocator::Provide(nullptr);
}

void DemoApplication::draw()
{
    auto* renderer = (Engine::VulkanRenderAPI*)Engine::RenderAPIServiceLocator::Service().get();
    auto& mRenderer = *renderer;
    
	auto swapChain = mWindow->GetSwapChain();
	const auto imageIndex = swapChain->SwapBuffers();

	// ------- Update Uniform buffer object ----------
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	CameraTransform transform;
	//transform.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform.view = glm::lookAt(mCameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform.projection = glm::perspective(glm::radians(68.0f), (float)::width / 3.0f / (float)::height, 0.1f, 10.0f);
	 
	transform.projection[1][1] *= -1;

	const auto vkunibuff = (Renderer::BufferVK*)(mUniformBuffers[imageIndex]->GetGpuResource());

	void* dataPtr{ nullptr };
	LowVK::MapMemory(vkunibuff->memory, 0, VkDeviceSize(sizeof(CameraTransform)), 0, &dataPtr);
	memcpy(dataPtr, &transform, (size_t)sizeof(CameraTransform));
	LowVK::UnmapMemory(vkunibuff->memory);

	//// ----------------- end of update ubo ------------
    
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { mImageAvailableSemaphore };

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &mCommandBuffers[imageIndex];
    
    VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    
	auto presentQueue = mRenderer.GetGraphicsQueue();
	LowVK::QueueSubmit(presentQueue, { submitInfo }, {});
    
	swapChain->Present();
}
