#include "elf_progbits.h"
#include "elf_section_header.h"
#include "utils/serialize.h"
#include "utils/term.h"

#include "elf.h"
#include <sys/mman.h>
#include <iostream>
#include <iomanip>
#include <cstdio>

using namespace boost;
using namespace serialization;
using namespace std;


template <typename Archiver>
shared_ptr<elf_progbits> elf_progbits::read(Archiver &AR,
                                        elf_section_header const &sh) {
  // Check section type
  if (!(sh.get_type() & SHT_PROGBITS)) {
    // This is not a PROGBITS section
    return shared_ptr<elf_progbits>();
  }

  // Reserve enough buffer
  shared_ptr<elf_progbits> result(new elf_progbits());
  // TODO: Align.
  result->buf_size = sh.get_size();
  if (result->buf_size > 0) {
    result->buf = (char *)mmap(0, result->buf_size, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  }

  // Check map success.
  if (result->buf == MAP_FAILED) {
    return shared_ptr<elf_progbits>();
  }

  // Save section_header
  result->section_header = shared_ptr<elf_section_header const>(&sh);

  // Read the buffer from string tab
  AR.seek(sh.get_offset(), true);
  AR.prologue(sh.get_size());
  AR.read_bytes(result->buf, sh.get_size());
  AR.epilogue(sh.get_size());

  if (!AR) {
    // Unable to read the progbits tab
    return shared_ptr<elf_progbits>();
  }

  return result;
}

char const *elf_progbits::memory_protect() const {
  int protect_type = PROT_READ;
  if (this->section_header->get_flags() & SHF_WRITE) {
    protect_type &= PROT_WRITE;
  }
  if (this->section_header->get_flags() &SHF_EXECINSTR) {
    protect_type &= PROT_EXEC;
  }
  if (buf_size > 0) {
    int ret = mprotect((void *)buf, buf_size, protect_type);
    if (ret == -1) {
      // FIXME: Throws excetion?
      std::cerr<<"Error: Can't mprotect."<<std::endl;
      return 0;
    }
  }
  return buf;
}

void elf_progbits::print() const {
  using namespace term::color;

  cout << endl << setw(79) << setfill('=') << '=' << endl;

  cout << light::white() << "PROGBITS:" <<
          section_header->get_name() << normal() << endl;

  cout << setw(79) << setfill('-') << '-' << endl << setfill(' ');

  cout << "Size: " << this->size() << endl;

  if (this->size() > 0) {
    char const *start = buf;
    char const *end = buf + buf_size - 1;
    char const *line_start = (char const *)((uint64_t)start & (~0xFLL));
    char const *line_end = (char const *)((uint64_t)end | (0xFLL));
#define CHECK_IN_BOUND(x) (start <= (x) && (x) <= end)
    for (char const *i = line_start; i <= line_end;) {
      // FIXME: Use std::cout.
      printf("%08LX: ", (unsigned long long)i);
      for (int k = 0; k < 2;++k) {
        printf(" ");
        for (int j = 0; j < 8; ++j) {
          if (CHECK_IN_BOUND(i)) {
            printf(" %.2x", (uint8_t)*i);
          } else {
            printf("   ");
          }
          ++i;
        }
      }
      printf("\n");
    }
#undef CHECK_IN_BOUND
  }
  cout << setw(79) << setfill('=') << '=' << endl;

}

template shared_ptr<elf_progbits>
elf_progbits::read(archive_reader_le &, elf_section_header const &);

template shared_ptr<elf_progbits>
elf_progbits::read(archive_reader_be &, elf_section_header const &);
