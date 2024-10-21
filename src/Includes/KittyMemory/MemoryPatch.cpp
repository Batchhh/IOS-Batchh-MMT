#include "MemoryPatch.hpp"
#include "KittyUtils.hpp"
#include <ptrauth.h>

#include <sys/mman.h>
#include <unistd.h>

// Helper functions for PAC
void* stripPAC(void* ptr) {
    return ptrauth_strip(ptr, ptrauth_key_function_pointer);
}

void* signPointer(void* ptr, void* context) {
    return ptrauth_sign_unauthenticated(ptr, ptrauth_key_function_pointer, context);
}

// Helper function to change memory protection
bool changeMemoryProtection(void* address, size_t size, int protection) {
    uintptr_t page_start = reinterpret_cast<uintptr_t>(address) & ~(getpagesize() - 1);
    return mprotect(reinterpret_cast<void*>(page_start), size, protection) == 0;
}

MemoryPatch::MemoryPatch()
    : _address(0), _size(0)
{
    _orig_code.clear();
    _patch_code.clear();
}

MemoryPatch::~MemoryPatch()
{
    _orig_code.clear();
    _patch_code.clear();
}

MemoryPatch::MemoryPatch(uintptr_t absolute_address,
                         const void *patch_code, size_t patch_size)
    : _address(0), _size(0)
{
    _orig_code.clear();
    _patch_code.clear();

    if (absolute_address == 0 || !patch_code || patch_size < 1)
        return;

    _address = absolute_address;
    _size = patch_size;

    _orig_code.resize(patch_size);
    _patch_code.resize(patch_size);

    void* stripped_address = stripPAC(reinterpret_cast<void*>(_address));

    // initialize patch & backup current content
    if (KittyMemory::memRead(&_patch_code[0], patch_code, patch_size) != KittyMemory::SUCCESS ||
        KittyMemory::memRead(&_orig_code[0], stripped_address, patch_size) != KittyMemory::SUCCESS) {
        // Handle error
    }
}

MemoryPatch::MemoryPatch(const char *fileName, uintptr_t address,
                         const void *patch_code, size_t patch_size)
    : _address(0), _size(0)
{
    _orig_code.clear();
    _patch_code.clear();

    if (address == 0 || !patch_code || patch_size < 1)
        return;

    _address = KittyMemory::getAbsoluteAddress(fileName, address);
    if (_address == 0)
        return;

    _size = patch_size;

    _orig_code.resize(patch_size);
    _patch_code.resize(patch_size);

    void* stripped_address = stripPAC(reinterpret_cast<void*>(_address));

    // initialize patch & backup current content
    if (KittyMemory::memRead(&_patch_code[0], patch_code, patch_size) != KittyMemory::SUCCESS ||
        KittyMemory::memRead(&_orig_code[0], stripped_address, patch_size) != KittyMemory::SUCCESS) {
        // Handle error
    }
}

MemoryPatch MemoryPatch::createWithHex(const char *fileName, uintptr_t address, std::string hex)
{
    MemoryPatch patch;

    if (address == 0 || !KittyUtils::validateHexString(hex))
        return patch;

    patch._address = KittyMemory::getAbsoluteAddress(fileName, address);
    if (patch._address == 0)
        return patch;

    patch._size = hex.length() / 2;

    patch._orig_code.resize(patch._size);
    patch._patch_code.resize(patch._size);

    void* stripped_address = stripPAC(reinterpret_cast<void*>(patch._address));

    // initialize patch
    KittyUtils::fromHex(hex, &patch._patch_code[0]);

    // backup current content
    if (KittyMemory::memRead(&patch._orig_code[0], stripped_address, patch._size) != KittyMemory::SUCCESS) {
        // Handle error
    }
    return patch;
}

MemoryPatch MemoryPatch::createWithHex(uintptr_t absolute_address, std::string hex)
{
    MemoryPatch patch;

    if (absolute_address == 0 || !KittyUtils::validateHexString(hex))
        return patch;

    patch._address = absolute_address;
    patch._size = hex.length() / 2;

    patch._orig_code.resize(patch._size);
    patch._patch_code.resize(patch._size);

    void* stripped_address = stripPAC(reinterpret_cast<void*>(patch._address));

    // initialize patch
    KittyUtils::fromHex(hex, &patch._patch_code[0]);

    // backup current content
    if (KittyMemory::memRead(&patch._orig_code[0], stripped_address, patch._size) != KittyMemory::SUCCESS) {
        // Handle error
    }
    return patch;
}

bool MemoryPatch::isValid() const
{
    return (_address != 0 && _size > 0 && _orig_code.size() == _size && _patch_code.size() == _size);
}

size_t MemoryPatch::get_PatchSize() const
{
    return _size;
}

uintptr_t MemoryPatch::get_TargetAddress() const
{
    return _address;
}

bool MemoryPatch::Restore()
{
    if (!isValid()) return false;

    void* stripped_address = stripPAC(reinterpret_cast<void*>(_address));
    if (!changeMemoryProtection(stripped_address, _size, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        // Handle error
        return false;
    }

    bool success = KittyMemory::memWrite(stripped_address, &_orig_code[0], _size) == KittyMemory::SUCCESS;

    if (!changeMemoryProtection(stripped_address, _size, PROT_READ | PROT_EXEC)) {
        // Handle error
    }

    return success;
}

bool MemoryPatch::Modify()
{
    if (!isValid()) return false;

    void* stripped_address = stripPAC(reinterpret_cast<void*>(_address));
    if (!changeMemoryProtection(stripped_address, _size, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        // Handle error
        return false;
    }

    bool success = KittyMemory::memWrite(stripped_address, &_patch_code[0], _size) == KittyMemory::SUCCESS;

    if (!changeMemoryProtection(stripped_address, _size, PROT_READ | PROT_EXEC)) {
        // Handle error
    }

    return success;
}

std::string MemoryPatch::get_CurrBytes() const
{
    if (!isValid()) return "";
  
    void* stripped_address = stripPAC(reinterpret_cast<void*>(_address));
    return KittyMemory::read2HexStr(stripped_address, _size);
}

std::string MemoryPatch::get_OrigBytes() const
{
    if (!isValid()) return "";
  
    return KittyMemory::read2HexStr(_orig_code.data(), _orig_code.size());
}

std::string MemoryPatch::get_PatchBytes() const
{
    if (!isValid()) return "";
  
    return KittyMemory::read2HexStr(_patch_code.data(), _patch_code.size());
}
