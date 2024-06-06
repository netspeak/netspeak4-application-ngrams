// Properties.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_PROPERTIES_HPP
#define NETSPEAK_INVERTEDINDEX_PROPERTIES_HPP

#include <filesystem>
#include <ostream>
#include <string>

#include "Configuration.hpp"

namespace netspeak::invertedindex {

struct Properties {
  static constexpr uint32_t k_version_number = 103; // 1.3.x

  Properties();
  Properties(const Properties& rhs);

  static const std::string default_filename();

  void print(std::ostream& os) const;

  void read(const std::filesystem::path& path);
  void write(const std::filesystem::path& path);

  Properties& operator=(const Properties& rhs);

  value_sorting_type value_sorting;
  uint32_t version_number;
  char value_type[64];
  uint64_t value_count;
  uint64_t total_size;
  uint64_t key_count;
};

std::ostream& operator<<(std::ostream& os, const Properties& properties);

} // namespace netspeak::invertedindex

#endif // NETSPEAK_INVERTEDINDEX_PROPERTIES_HPP
