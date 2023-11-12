//
// Created by nguye on 11/12/2023.
//
#include "patch.h"

bool MemoryPatch::unprotect(void*  offset, unsigned  int size) {
    int result = mprotect(offset, size, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (result == 0) {
        LOG_E("unprotect oke");
        return true;
    } else {
#ifdef IS_DEBUG
        int error = errno;
        const char* errorMessage = strerror(error);
        LOG_E("%s", errorMessage);
#endif
        return false;
    }
}

bool MemoryPatch::protect(void*  offset, unsigned  int size) {
    int result = mprotect(offset, size, PROT_READ | PROT_EXEC);
    if (result == 0) {
        LOG_E("protect oke");
        return true;
    } else {
#ifdef IS_DEBUG
        int error = errno;
        const char* errorMessage = strerror(error);
        LOG_E("%s", errorMessage);
#endif
        return false;
    }
}

MemoryPatch::MemoryPatch(uintptr_t offset, const std::vector<uint8_t> &patch) {
    _offset = offset;
    _patch = patch;
    _isActive = false;
}

MemoryPatch::MemoryPatch() {
}

MemoryPatch::MemoryPatch(const MemoryPatch &memoryPatch) {
    _offset = memoryPatch._offset;
    _patch = memoryPatch._patch;
    _backup = memoryPatch._backup;
    _isActive = memoryPatch._isActive;
}

void MemoryPatch::execute() {
    if (_isActive) {
        return;
    }
    size_t sizePatch = _patch.size();
    _backup = std::vector<unsigned char>(_patch.size());
    memcpy((void*)&_backup[0], (void*)_offset, sizePatch);
    LOG_E("backup %p", (uintptr_t)_backup[0]);

    // write
    memcpy((void*)_offset, (void*)&_patch[0], sizePatch);
    LOG_E("write %p", (uintptr_t )_patch[0]);
    _isActive = true;
}

void MemoryPatch::restore() {
    if (!_isActive) {
        return;
    }
    size_t sizeBackup = _backup.size();
    memcpy((void*)_offset, (void*)&_backup[0], sizeBackup);
    LOG_E("restore %p", (uintptr_t)_backup[0]);
    _isActive = false;
}

void MemoryPatch::executeAll(std::vector<MemoryPatch> &mps) {
    for (MemoryPatch& mp: mps) {
        mp.execute();
    }
}

void MemoryPatch::restoreAll(std::vector<MemoryPatch> &mps) {
    for (MemoryPatch& mp: mps) {
        mp.restore();
    }
}

bool MemoryPatch::isActive() {
    return _isActive;
}

