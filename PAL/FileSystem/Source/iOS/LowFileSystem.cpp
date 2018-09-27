#include "LowFileSystem.h"

#import <Foundation/Foundation.h>
#import <PAL/Filesystem/NSFileManager+EngineWritable.h>

#include <iostream>

using namespace PAL::FileSystem;

std::shared_ptr<IFileSystem> FileSystemServiceLocator::mService = nullptr;

namespace PAL::FileSystem
{
    class MacOSFileSystem : public IFileSystem
    {
    public:
        virtual void Initialize() override
        {
            std::cout << "Created MacOS FileSystem" << std::endl;
            mFileManager = [NSFileManager defaultManager];
            mWritableFolder = [[[NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) firstObject] stringByAppendingPathComponent:@"sygic"] copy];
            mReadableFolder = [[[NSBundle mainBundle] bundlePath] copy];
            
            NSLog(@"Read-only folder is: %@", mReadableFolder);
            NSLog(@"Writable folder is: %@", mWritableFolder);
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
            std::cout << "Destroyed MacOS FileSystem" << std::endl;
            mFileManager = nil;
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
