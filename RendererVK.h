#pragma once

#include "Vertex.h"
#include <memory>
#include <iostream>
#include <vulkan/vulkan.hpp>
#include "Buffer.h"

namespace renderer
{
    class Renderer
    {
    public:
        virtual ~Renderer() = default;
        
        template<typename ObjectCreateFunc>
        void CreateGpuObject(const char* objectName, ObjectCreateFunc&& create)
        {
            create();
            std::cout << "Created GPU object: " << objectName << std::endl;
        }
        
        void OnGpuObjectCreated()
        {
            
        }
    };
    
    class RendererVK : public Renderer
    {
    public:
        RendererVK() = default;
        
        void Initialize();
        void SetDevice(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::CommandPool& cmdPool, const vk::Queue& queue);
        
        void CreateBuffer(const BufferDesc& desc, const BufferData& data, std::shared_ptr<IBuffer>& bufferObject);
        
    private:
        void QueryInstanceExtensions();
        void QueryInstanceLayers();
        
    private:
        std::shared_ptr<vk::Device> mDevice;
        vk::PhysicalDevice mPhysicalDevice;
        vk::CommandPool mCmdPool;
        vk::Queue mQueue;
        
        // Setup
        std::vector<vk::ExtensionProperties> mInstanceExtensions;
        std::vector<vk::LayerProperties> mInstanceLayers;
    };
}
