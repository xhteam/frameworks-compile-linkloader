#include "ELFSectionHeader.h"

#include <elf.h>

// ARM Section Header Type Definitions

// TODO: These definitions are not defined in external/elfutils/libelf/
// elf.h.  So we to this by ourself.  Maybe we should update elf.h
// instead.

#ifndef SHT_ARM_EXIDX
#define SHT_ARM_EXIDX (SHT_LOPROC + 1)
#endif

#ifndef SHT_ARM_PREEMPTMAP
#define SHT_ARM_PREEMPTMAP (SHT_LOPROC + 2)
#endif

#ifndef SHT_ARM_ATTRIBUTES
#define SHT_ARM_ATTRIBUTES (SHT_LOPROC + 3)
#endif

char const *ELFSectionHeaderHelperMixin::getSectionTypeStr(uint32_t type) {
  switch (type) {
    default: return "(UNKNOWN)";

#define CASE(TYPE) \
    case SHT_##TYPE: return #TYPE;

    // General section header type
    CASE(NULL) CASE(PROGBITS) CASE(SYMTAB) CASE(STRTAB) CASE(RELA) CASE(HASH)
    CASE(DYNAMIC) CASE(NOTE) CASE(NOBITS) CASE(REL) CASE(SHLIB)
    CASE(DYNSYM) CASE(INIT_ARRAY) CASE(FINI_ARRAY) CASE(PREINIT_ARRAY)
    CASE(GROUP) CASE(SYMTAB_SHNDX) CASE(LOOS) CASE(HIOS) CASE(LOPROC)
    CASE(HIPROC) CASE(LOUSER) CASE(HIUSER)

    // ARM-specific section header type
    CASE(ARM_EXIDX) CASE(ARM_PREEMPTMAP) CASE(ARM_ATTRIBUTES)

#undef CASE
  }
}