#pragma once

#include <cstdint>

namespace Renderer
{
    enum BufferUsage : uint32_t
    {
        Static = 0x01,
        Staging = 0x02,
        TransferSrc = 0x04,
        TransferDest = 0x08,
    };
    
    enum BufferBindFlags : uint32_t
    {
        VertexBuffer = 0x01,
        IndexBuffer = 0x02,
        UniformBuffer = 0x04
    };
    
    class BufferDesc
    {
    public:
        BufferDesc() = default;
        BufferDesc(BufferUsage buffUsage, BufferBindFlags bindFlags)
        : usage(buffUsage), flags(bindFlags)
        {}
        
        uint32_t usage{ 0 };
        uint32_t flags{ 0 };
    };
    
    class BufferData
    {
    public:
        BufferData() = default;
        BufferData(void* dataPtr, uint32_t dataSize)
        : data(dataPtr), size(dataSize)
        {}
        
        void* data{ nullptr };
        uint32_t size{ 0 };
    };
    
    enum class ImageFormat
    {
        RGBA8,
        DEPTH
    };
    
    enum class ImageUsage
    {
        Sampled,
        DepthAttachment
    };
}
