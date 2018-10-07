#include <PAL/FileSystem/File.h>
#include <PAL/FileSystem/LowFileSystem.h>

using namespace PAL::FileSystem;

File::File(const std::string& path)
: mPath(path)
{
    
}

void File::Open(EFileAccessMode mode)
{
    mHandle = FileSystemServiceLocator::Service()->FileOpen(mPath, mode);
    if(!mHandle)
    {
        throw FileException();
    }
}

void File::ReadBytes(uint32_t bytesToRead)
{
    const uint32_t bytesRead = FileSystemServiceLocator::Service()->FileRead(mHandle, mBuffer.get(), bytesToRead);
    if(bytesToRead != bytesRead)
    {
        throw FileException();
    }
}

void File::Close()
{
    if (mHandle)
    {
        FileSystemServiceLocator::Service()->FileClose(mHandle);
        mHandle = nullptr;
    }
    else
    {
        throw FileException();
    }
}

bool File::IsOpened() const
{
    return mHandle != nullptr;
}

const std::vector<uint8_t>& File::Read()
{
    if(!IsOpened())
        Open(EFileAccessMode::Read);
    
    const uint32_t fileSize = FileSystemServiceLocator::Service()->FileGetSize(mHandle);
    
    std::vector<uint8_t> data;
    data.resize(fileSize);
    
    mBuffer = std::make_unique<std::vector<uint8_t>>(data);
    
    const uint32_t bytesRead = FileSystemServiceLocator::Service()->FileRead(mHandle, mBuffer->data(), fileSize);
    if(fileSize != bytesRead)
    {
        mBuffer.release();
        Close();
        throw FileException();
    }
    
    Close();
    
    return *mBuffer.get();
}
