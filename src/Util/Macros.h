#pragma once

#include <substrate.h>
#include <mach-o/dyld.h>

#import "Obfuscate.h"

#import "../Includes/KittyMemory/writeData.hpp"
#import "../Includes/KittyMemory/MemoryPatch.hpp"

#define timer(sec) dispatch_after(dispatch_time(DISPATCH_TIME_NOW, sec * NSEC_PER_SEC), dispatch_get_main_queue(), ^
#define HOOK(offset, ptr, orig) MSHookFunction((void *)getRealOffset(offset), (void *)ptr, (void **)&orig)
#define HOOK_NO_ORIG(offset, ptr) MSHookFunction((void *)getRealOffset(offset), (void *)ptr, NULL)

#define HOOKSYM(sym, ptr, org) MSHookFunction((void*)dlsym((void *)RTLD_DEFAULT, sym), (void *)ptr, (void **)&org)
#define HOOKSYM_NO_ORIG(sym, ptr)  MSHookFunction((void*)dlsym((void *)RTLD_DEFAULT, sym), (void *)ptr, NULL)
#define getSym(symName) dlsym((void *)RTLD_DEFAULT, symName)

// Convert hex color to UIColor, usage: For the color #BD0000 you'd use: UIColorFromHex(0xBD0000)
#define UIColorFromHex(hexColor) [UIColor colorWithRed:((float)((hexColor & 0xFF0000) >> 16))/255.0 green:((float)((hexColor & 0xFF00) >> 8))/255.0 blue:((float)(hexColor & 0xFF))/255.0 alpha:1.0]

uint64_t getRealOffset(uint64_t offset){
	return KittyMemory::getAbsoluteAddress(BINARY_NAME, offset);
}

void patchOffset(uint64_t offset, std::string hexBytes) {
	MemoryPatch patch = MemoryPatch::createWithHex(BINARY_NAME, offset, hexBytes);
	if(!patch.isValid() && DEBUG_MODE){
        [APMenu showPopup:@"Invalid patch" description:[NSString stringWithFormat:@"Failing offset: 0x%llx, please re-check the hex you entered.", offset]];
		return;
	}
	if(!patch.Modify() && DEBUG_MODE) {
	  [APMenu showPopup:@"Invalid patch" description:[NSString stringWithFormat:@"Something went wrong while patching this offset: 0x%llu.", offset]];
    }
}