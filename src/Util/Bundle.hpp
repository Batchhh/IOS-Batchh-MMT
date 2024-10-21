#pragma once

namespace Bundle
{
    static inline const char* getFrameWorkPath(const char* NAME) {
        NSString *appPath = [[NSBundle mainBundle] bundlePath];
        NSString *binaryPath = [NSString stringWithFormat:@"%s", NAME];
        if ([binaryPath isEqualToString:@"UnityFramework"])
        {
            binaryPath = [appPath stringByAppendingPathComponent:@"Frameworks/UnityFramework.framework/UnityFramework"];
        }
        else
        {
            binaryPath = [appPath stringByAppendingPathComponent:binaryPath];
        }
        return [binaryPath UTF8String];
    }
}
