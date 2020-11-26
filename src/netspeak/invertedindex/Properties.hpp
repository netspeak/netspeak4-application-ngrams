// Properties.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_PROPERTIES_HPP
#define NETSPEAK_INVERTEDINDEX_PROPERTIES_HPP

#include <ostream>
#include <string>

#include <boost/filesystem.hpp>

#include "netspeak/invertedindex/Configuration.hpp"
#include "netspeak/util/systemio.hpp"

namespace netspeak {
namespace invertedindex {

struct Properties {
  static const uint32_t k_version_number = 103; // 1.3.x

  Properties()
      : value_sorting(value_sorting_type::disabled),
        version_number(k_version_number),
        value_count(),
        total_size(),
        key_count() {
    std::memset(value_type, 0, sizeof(value_type));
  }

  Properties(const Properties& rhs)
      : value_sorting(rhs.value_sorting),
        version_number(rhs.version_number),
        value_count(rhs.value_count),
        total_size(rhs.total_size),
        key_count(rhs.key_count) {
    std::strncpy(value_type, rhs.value_type, sizeof(value_type));
  }

  static const std::string default_filename() {
    return "properties"; // alternative: static class constant
  }

  void print(std::ostream& os) const {
    os << "{\n  value_type : " << value_type
       << ",\n  value_sorting : " << util::to_string(value_sorting)
       << ",\n  version_number : " << version_number
       << ",\n  total_size : " << total_size
       << ",\n  value_count : " << value_count
       << ",\n  key_count : " << key_count << "\n}";
  }

  void read(const boost::filesystem::path& path) {
    FILE* rfs(util::fopen(path, "rb"));
    util::fread(this, sizeof(Properties), 1, rfs);
    util::fclose(rfs);
  }

  void write(const boost::filesystem::path& path) {
    FILE* wfs(util::fopen(path, "wb"));
    util::fwrite(this, sizeof(Properties), 1, wfs);
    util::fclose(wfs);
  }

  Properties& operator=(const Properties& rhs) {
    std::strncpy(value_type, rhs.value_type, sizeof(value_type));
    value_sorting = rhs.value_sorting;
    version_number = rhs.version_number;
    value_count = rhs.value_count;
    total_size = rhs.total_size;
    key_count = rhs.key_count;
    return *this;
  }

  value_sorting_type value_sorting;
  uint32_t version_number;
  char value_type[64];
  uint64_t value_count;
  uint64_t total_size;
  uint64_t key_count;
};

inline std::ostream& operator<<(std::ostream& os,
                                const Properties& properties) {
  if (os)
    properties.print(os);
  return os;
}

} // namespace invertedindex
} // namespace netspeak

#endif // NETSPEAK_INVERTEDINDEX_PROPERTIES_HPP
