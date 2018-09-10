#pragma once

#include "RendererBase.h"

#include <cstdint>
#include <cstddef>
#include <memory>

#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
private:                                    \
TypeName(const TypeName&) = delete;         \
void operator=(const TypeName&) = delete;

namespace renderer
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
    
    class RENDERER_API IBuffer
    {
    public:
        virtual ~IBuffer() = default;
        
        virtual void CopyData(std::shared_ptr<IBuffer>& srcBuffer, std::shared_ptr<IBuffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size) = 0;
		virtual void Map(const uint64_t offset, const uint64_t size, void* data) = 0;
		virtual void Release() = 0;
        
        //virtual void Map() {}
        //virtual void Unmap() {}
    };
}
