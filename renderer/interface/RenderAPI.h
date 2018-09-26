#pragma once

#include "RendererBase.h"
#include "BufferTypes.h"
#include <memory>
#include <vector>

namespace Renderer
{
    class RendererEffect;
    class Texture;
    class Buffer;
    class Effect;
}

namespace Renderer
{
    class CommandList
    {
    public:
        virtual ~CommandList() = default;
    };
    
    class RendererTexture
    {
    public:
        virtual ~RendererTexture() = default;
    };
    
    class RendererBuffer
    {
    public:
        virtual ~RendererBuffer() = default;
    };
    
    class RendererEffect
    {
    public:
        virtual ~RendererEffect() = default;
    };
}

namespace Engine
{
    class RENDERER_API IRenderAPI
    {
    public:
        virtual ~IRenderAPI() = default;
        
        virtual void Initialize(void* window) = 0;
        virtual void CreateShader(std::shared_ptr<Renderer::RendererEffect>& gpuEffect, const std::vector<char>& sourceCode) = 0;
        virtual void CreateTexture(Renderer::Texture& texture) = 0;
        virtual void CreateBuffer(const Renderer::BufferDesc& desc, const Renderer::BufferData& data, std::shared_ptr<Renderer::Buffer>& bufferObject) = 0;
        virtual void CopyBuffer(std::shared_ptr<Renderer::Buffer>& srcBuffer, std::shared_ptr<Renderer::Buffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size) = 0;
        virtual void CreateDescriptorSetLayout() = 0;
        virtual void CreatePipeline(Renderer::Effect& effect) = 0;
        
        virtual void Deinitialize() = 0;
    };

    class RENDERER_API RenderAPIServiceLocator
    {
    public:
        static void Provide(std::shared_ptr<IRenderAPI> service)
        {
            mService = std::move(service);
        }
        
        static std::shared_ptr<IRenderAPI> Service()
        {
            return mService;
        }
        
        static bool Available()
        {
            return !mService;
        }
        
    private:
        static std::shared_ptr<IRenderAPI> mService;
    };
}

