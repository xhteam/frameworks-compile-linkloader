#ifndef ELF_nobits_H
#define ELF_nobits_H

#include "elf_section.h"
#include "utils/serialize.h"

#include <boost/shared_ptr.hpp>
#include <vector>

class elf_nobits : public elf_section {
private:
  boost::shared_ptr<elf_section_header const> section_header;
  char *buf;
  size_t buf_size;

private:
  elf_nobits() { }

public:
  template <typename Archiver>
  static boost::shared_ptr<elf_nobits> read(Archiver &AR,
                                            elf_section_header const &);

  char &operator[](size_t index) {
    return buf[index];
  }

  char const &operator[](size_t index) const {
    return buf[index];
  }

  size_t size() const {
    return buf_size;
  }

  char const *memory_protect() const;

  virtual void print() const;
};

extern template boost::shared_ptr<elf_nobits>
elf_nobits::read(serialization::archive_reader_le &,
                 elf_section_header const &);

extern template boost::shared_ptr<elf_nobits>
elf_nobits::read(serialization::archive_reader_be &,
                 elf_section_header const &);

#endif // ELF_nobits_H
