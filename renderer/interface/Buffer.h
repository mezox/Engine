#pragma once

#include "RendererBase.h"

#include <cstdint>
#include <cstddef>
#include <memory>

#include "RenderAPI.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
private:                                    \
TypeName(const TypeName&) = delete;         \
void operator=(const TypeName&) = delete;

namespace Renderer
{
	class RendererBuffer;
    
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
