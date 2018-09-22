#pragma once

#include "RendererBase.h"

#include <cstdint>
#include <cstddef>
#include <memory>

#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
private:                                    \
TypeName(const TypeName&) = delete;         \
void operator=(const TypeName&) = delete;

namespace Renderer
{
	class RendererBuffer;

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
    
    class Buffer
    {
    public:
        virtual ~Buffer() = default;

		const RendererBuffer* GetGpuResource() const { return mGpuBuffer.get(); }
		void SetGpuResource(std::unique_ptr<RendererBuffer> buffer) { mGpuBuffer = std::move(buffer); }

		void MapMemory(void* data, uint32_t dataSize);

	protected:
		std::unique_ptr<RendererBuffer> mGpuBuffer;
    };
}
