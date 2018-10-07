#pragma once

#include "FileSystemBase.h"
#include "FileTypes.h"

#include <memory>
#include <string>
#include <cstdint>

namespace PAL::FileSystem
{        
    class FILESYSTEM_API IFileSystem
    {
    public:
        virtual ~IFileSystem() = default;
        
        virtual void Initialize() = 0;
        virtual std::string GetFilePath(const std::string& file) const = 0;
        virtual FileHandle FileOpen(const std::string& filePath, EFileAccessMode mode) const = 0;
        virtual uint32_t FileRead(FileHandle handle, void* buffer, uint32_t bytesToRead) const = 0;
        virtual uint32_t FileGetSize(FileHandle handle) const = 0;
        virtual void FileClose(FileHandle handle) const = 0;
        virtual void DeInitialize() = 0;
    };
    
    FILESYSTEM_API std::shared_ptr<IFileSystem> CreateFileSystemService();
    
    class FILESYSTEM_API FileSystemServiceLocator
    {
    public:
        static void Provide(std::shared_ptr<IFileSystem> service)
        {
            mService = std::move(service);
        }
        
        static std::shared_ptr<IFileSystem> Service()
        {
            return mService;
        }
        
        static bool Available()
        {
            return !mService;
        }
        
    private:
        static std::shared_ptr<IFileSystem> mService;
    };
}
