#ifndef NETSPEAK_UTIL_STDPATHFORMATTER_HPP
#define NETSPEAK_UTIL_STDPATHFORMATTER_HPP

/**
 * @file std_path_formatter.hpp
 * @brief Defines an std::formatter for std::filesystem::path. MUST be deleted when C++26 is available with that
 * feature.
 */

#include <filesystem>
#include <format>
#include <string>

template <>
struct std::formatter<std::filesystem::path> : public std::formatter<std::string> {
public:
  auto format(std::filesystem::path path, format_context& ctx) const {
    return formatter<string>::format(std::format("{}", path.string()), ctx);
  }
};

template <>
struct std::formatter<std::filesystem::directory_entry> : public std::formatter<std::string> {
public:
  auto format(std::filesystem::directory_entry entry, format_context& ctx) const {
    return formatter<string>::format(std::format("{}", entry.path()), ctx);
  }
};

#endif