#include "RendererVK.h"

#include "BufferImpl.h"

using namespace renderer;

void RendererVK::Initialize()
{
    QueryInstanceExtensions();
    QueryInstanceLayers();
}

void RendererVK::SetDevice(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::CommandPool& cmdPool, const vk::Queue& queue)
{
    mDevice = std::make_shared<vk::Device>(device);
    mPhysicalDevice = physicalDevice;
    mQueue = queue;
    mCmdPool = cmdPool;
}


void RendererVK::CreateBuffer(const BufferDesc& desc, const BufferData& data, std::shared_ptr<IBuffer>& bufferObject)
{
    CreateGpuObject("Triangle vertex buffer", [&bufferObject, this, data, desc](){
       bufferObject = std::make_shared<renderer::Buffer>(mDevice, mPhysicalDevice, mCmdPool, mQueue, desc, data);
    });
}

void RendererVK::QueryInstanceExtensions()
{
    mInstanceExtensions = vk::enumerateInstanceExtensionProperties();
    std::vector<const char*> extensionNames(mInstanceExtensions.size(), nullptr);
    
    std::cout << mInstanceExtensions.size() << " instance extensions available: " << std::endl;
    
    for(size_t idx = 0; idx < mInstanceExtensions.size(); ++idx)
    {
        std::cout << mInstanceExtensions[idx].extensionName << "\n";
        extensionNames[idx] = (&mInstanceExtensions[idx].extensionName[0]);
    }
}

void RendererVK::QueryInstanceLayers()
{
    mInstanceLayers = vk::enumerateInstanceLayerProperties();
    
    std::cout << mInstanceLayers.size() << " instance layers available: " << std::endl;
    
    for(const auto& layer : mInstanceLayers)
    {
        std::cout << layer.layerName << "\n";
    }
}
