#pragma once

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
    };
    
    class BufferDesc
    {
    public:
        uint32_t usage;
        uint32_t flags;
    };
    
    class BufferData
    {
    public:
        void* data{ nullptr };
        uint32_t size;
    };
    
    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;
        
        virtual void CopyData(std::shared_ptr<IBuffer>& srcBuffer, std::shared_ptr<IBuffer>& dstBuffer, const size_t srcOffset, const size_t dstOffset, const size_t size) = 0;
        
        //virtual void Map() {}
        //virtual void Unmap() {}
        //virtual void* GetHandle() const { return nullptr; }
    };
}
