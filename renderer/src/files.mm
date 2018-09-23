#import <Cocoa/Cocoa.h>

const char* bundlePath(const char* file, const char* ext)
{
    NSString *myPath = [[NSBundle mainBundle]pathForResource:[NSString stringWithUTF8String:file] ofType:[NSString stringWithUTF8String:ext]];
    NSString *myFile = [[NSString alloc]initWithContentsOfFile:myPath encoding:NSUTF8StringEncoding error:nil];
    return [myPath UTF8String];
}
