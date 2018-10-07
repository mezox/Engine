#include "LowFileSystem.h"

#import <Foundation/Foundation.h>

using namespace PAL::FileSystem;

std::shared_ptr<IFileSystem> FileSystemServiceLocator::mService = nullptr;

namespace PAL::FileSystem
{
    class MacOSFileSystem : public IFileSystem
    {
    public:
        virtual void Initialize() override
        {
            mFileManager = [NSFileManager defaultManager];
            mWritableFolder = [[[NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) firstObject] stringByAppendingPathComponent:@"sygic"] copy];
            mReadableFolder = [[[NSBundle mainBundle] bundlePath] copy];
            
            NSLog(@"Read-only folder is: %@", mReadableFolder);
            NSLog(@"Writable folder is: %@", mWritableFolder);
        }
        
        virtual FileHandle FileOpen(const std::string& filePath, EFileAccessMode mode) const override
        {
            NSString* filePathNS = [NSString stringWithUTF8String:filePath.c_str()];
            
            if([mFileManager fileExistsAtPath:filePathNS])
            {
                @autoreleasepool
                {
                    NSFileHandle *handle{ nil };
                    
                    if (mode == EFileAccessMode::Read)
                    {
                        handle = [NSFileHandle fileHandleForReadingAtPath:filePathNS];
                    }
                    
                    if (handle)
                    {
                        return (FileHandle)CFBridgingRetain(handle);
                    }
                }
            }
            
            return nullptr;
        }
        
        virtual uint32_t FileRead(FileHandle handle, void* buffer, uint32_t bytesToRead) const override
        {
            NSFileHandle* file = (__bridge NSFileHandle*)handle;
            
            uint32_t readBytes{ 0 };
            
            @autoreleasepool
            {
                NSData* data = [file readDataOfLength:bytesToRead];
                
                if( data )
                {
                    [data getBytes:buffer length:bytesToRead];
                    readBytes = (uint32_t)data.length;
                }
            }
            
            return readBytes;
        }
        
        virtual uint32_t FileGetSize(FileHandle handle) const override
        {
            NSFileHandle* file = (__bridge NSFileHandle*)handle;
            
            uint64_t nOriginalPos = [file offsetInFile];
            uint32_t dwRet = (uint32_t)[file seekToEndOfFile];
            [file seekToFileOffset:nOriginalPos];
            
            return dwRet;
        }
        
        virtual void FileClose(FileHandle handle) const override
        {
            CFBridgingRelease(handle);
        }
        
        virtual std::string GetFilePath(const std::string& file) const override
        {
            NSString* strName = [NSString stringWithUTF8String:file.c_str()];
            
            if([mFileManager fileExistsAtPath:strName])
                return [[NSString stringWithFormat:@"%@/%@", [mFileManager currentDirectoryPath], strName] UTF8String];
            
            return file;
        }
        
        virtual void DeInitialize() override
        {
            mFileManager = nil;
            mWritableFolder = nil;
            mReadableFolder = nil;
        }
        
    private:
        std::string GetRealPath(const std::string& filePath) const
        {
            const std::string absolutePath("file://");
            
            if(filePath.find(absolutePath) == 0)
            {
                return [[NSString stringWithUTF8String:filePath.substr(absolutePath.size(), std::string::npos).c_str()] UTF8String];
            }
            
            NSString* strFile = [NSString stringWithUTF8String:filePath.c_str()];
            strFile = [strFile stringByReplacingOccurrencesOfString:mWritableFolder withString:@""];
            strFile = [strFile stringByReplacingOccurrencesOfString:mReadableFolder withString:@""];
            
            return [[mWritableFolder stringByAppendingPathComponent:strFile] UTF8String];
        }
        
    private:
        NSFileManager* mFileManager{ nil };
        NSString* mWritableFolder{ nil };
        NSString* mReadableFolder{ nil };
    };
}

std::shared_ptr<IFileSystem> PAL::FileSystem::CreateFileSystemService()
{
    return std::make_shared<MacOSFileSystem>();
}
