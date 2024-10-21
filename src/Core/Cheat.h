#pragma once

namespace Cheat {
    void handle() {
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            //here add your hooks.

            //normal MSHook
            //HOOK(offset, ptr, orig);

            //IL2CPP Hooks
            //IL2CPP::Helper::HookStaticMethod(const int &className, const int &methodName, int argsCount, Ret (*hookedFunction)(Args...), Ret (*originalFunction)(Args...))
        });
    }
    void Init_Resolver() {
        bool init = IL2CPP::Initialize(true, WAIT_TIME_SEC, Bundle::getFrameWorkPath(BINARY_NAME));
        if (!init && DEBUG_MODE) Alert::showSuccess([NSString stringWithFormat:@"Resolver is having problems!\nCheck BINARY_NAME!\nPossible obfuscation, change the m_ExportObfuscation variable!"]);
    }
}