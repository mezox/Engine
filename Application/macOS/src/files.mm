#import <Cocoa/Cocoa.h>

const char* bundlePath(const char* file)
{
    NSString *myPath = [[NSBundle mainBundle]pathForResource:[NSString stringWithUTF8String:file] ofType:@"spv"];
    NSString *myFile = [[NSString alloc]initWithContentsOfFile:myPath encoding:NSUTF8StringEncoding error:nil];
    return [myPath UTF8String];
}
