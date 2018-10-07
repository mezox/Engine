#pragma once

#include "FileSystemBase.h"
#include "FileTypes.h"

#include <string>
#include <memory>
#include <cstdint>
#include <vector>

namespace PAL::FileSystem
{
    class FILESYSTEM_API File
    {
    public:
        explicit File(const std::string& path);
        
        void Open(EFileAccessMode mode);
        void ReadBytes(uint32_t bytesToRead);
        const std::vector<uint8_t>& Read();
        void Close();
        bool IsOpened() const;
        
    private:
        FileHandle mHandle{ nullptr };
        std::string mPath;
        std::unique_ptr<std::vector<uint8_t>> mBuffer;
    };
}
