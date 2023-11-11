//
// Created by nguye on 11/11/2023.
//

#ifndef PIGMOD_WELF_H
#define PIGMOD_WELF_H
#include <cstdint>
#include <cstring>
#include <linux/elf.h>

template<typename FUNCTION>
inline void LOOP_INLINE(int n, FUNCTION f) {
    for (int i = 0; i < n; ++i) {
        f();
    }
}

#define NOP_10 LOOP_INLINE(10, []() { \
    __asm __volatile \
            ( \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
            "nop                                \n\t" \
    ); \
})

#define LOOP_WTF LOOP_INLINE(10, []() { \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
    NOP_10; \
})

// Function to find a symbol in the library data
const Elf64_Sym* FindSymbol(const unsigned char* libData, const char* symbolName) {
    const Elf64_Ehdr* header = reinterpret_cast<const Elf64_Ehdr*>(libData);

    // Check if it's an ELF file
    if (header->e_ident[0] != 0x7F || strncmp(reinterpret_cast<const char*>(header->e_ident + 1), "ELF", 3) != 0) {
        return nullptr;
    }

    const Elf64_Shdr* sectionHeader = reinterpret_cast<const Elf64_Shdr*>(libData + header->e_shoff);
    const char* stringTable = reinterpret_cast<const char*>(libData + sectionHeader[header->e_shstrndx].sh_offset);

    // Iterate through the sections
    for (uint16_t i = 0; i < header->e_shnum; ++i) {
        if (sectionHeader[i].sh_type == SHT_SYMTAB || sectionHeader[i].sh_type == SHT_DYNSYM) {
            const Elf64_Sym* symbolTable = reinterpret_cast<const Elf64_Sym*>(libData + sectionHeader[i].sh_offset);
            const char* symbolNames = ( const char*)(libData + sectionHeader[sectionHeader[i].sh_link].sh_offset);

            for (uint16_t j = 0; j < sectionHeader[i].sh_size / sizeof(Elf64_Sym); ++j) {
                if (strcmp(symbolNames + symbolTable[j].st_name, symbolName) == 0) {
                    LOG_E("size %i", symbolTable[j].st_size);
                    return &(symbolTable[j]);
                }
            }
        }
    }

    return nullptr;
}


#endif //PIGMOD_WELF_H
