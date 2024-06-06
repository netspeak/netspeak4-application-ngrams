// systemio.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_UTIL_SYSTEMIO_HPP
#define NETSPEAK_UTIL_SYSTEMIO_HPP

#include <assert.h>

#include <climits>
#include <cstring>
#include <filesystem>
#include <format>
#include <netspeak/util/std_path_formatter.hpp>
#include <stdexcept>

/**
 * Functions to replace FILE* I/O to throw exceptions on failure.
 */
namespace netspeak::util {

namespace fs = std::filesystem;

inline bool is_hidden_file(const fs::path& path) {
  return path.filename().string().front() == '.';
}

/**
 * Converts the given 'path' to an absolute path using 'base' as base path.
 * If 'path' is already an absolute path it is returned without modification.
 */
inline fs::path make_absolute(const fs::path& path, const fs::path& base) {
  return path.is_absolute() ? path : (base / path);
}

inline void signal_error(const std::string& msg) {
  std::perror(msg.c_str());
  throw std::runtime_error(msg);
}

template <typename T>
void signal_error(const std::string& msg, const T& obj) {
  std::perror(msg.c_str());
  throw std::runtime_error(std::format("{} : {}", msg, obj));
}

inline uint64_t directory_size(const fs::path& dir) {
  uint64_t num_bytes = 0;
  const fs::directory_iterator end;
  for (fs::directory_iterator it(dir); it != end; ++it) {
    if (fs::is_regular_file(it->path())) {
      if (!is_hidden_file(it->path())) {
        num_bytes += fs::file_size(it->path());
      }
    } else if (fs::is_directory(it->path())) {
      num_bytes += directory_size(it->path());
    }
  }
  return num_bytes;
}

inline void fclose(FILE* fs) {
  if (fs == nullptr)
    return;
  if (std::fclose(fs) == -1) {
    signal_error("std::fclose failed");
  }
}

inline FILE* fopen(const fs::path& path, const std::string& mode) {
  FILE* fs(std::fopen(path.string().c_str(), mode.c_str()));
  if (fs == nullptr) {
    signal_error("std::fopen failed", path);
  }
  return fs;
}

inline void fread(void* data, size_t size, size_t count, FILE* fs) {
  // As it turns out, simply asserting nullptr is not enough. As the `std::fread`
  // spec clearly states:
  //
  // > f size or count is zero, fread returns zero and performs no other
  // > action.
  //
  // Since `data` may be the `data()` value of a vector, we have to take this
  // into account because `data()` may return nullptr for empty vectors depending
  // on the compiler.
  assert(data != nullptr || size == 0 || count == 0);
  assert(fs != nullptr);

  if (std::fread(data, size, count, fs) != count) {
    signal_error("std::fread failed");
  }
}

inline void fseek(FILE* fs, long offset, int origin) {
  assert(fs != nullptr);
  if (std::fseek(fs, offset, origin) != 0) {
    signal_error("std::fseek failed");
  }
}

inline void fwrite(const void* data, size_t size, size_t count, FILE* fs) {
  // Same as for `util::fread`
  assert(data != nullptr || size == 0 || count == 0);
  assert(fs != nullptr);

  if (std::fwrite(data, size, count, fs) != count) {
    signal_error("std::fwrite failed");
  }
}

inline size_t ftell(FILE* fs) {
  assert(fs != nullptr);
  const long offset(std::ftell(fs));
  if (offset == -1) {
    signal_error("std::ftell failed");
  }
  return offset;
}

inline void rewind(FILE* fs) {
  if (fs == nullptr)
    return;
  std::rewind(fs);
}

inline FILE* tmpfile() {
  FILE* fs(std::tmpfile());
  if (fs == nullptr) {
    signal_error("std::tmpfile failed");
  }
  return fs;
}

inline const fs::path tmpdir(const fs::path& parent_dir) {
  char tmpdir[PATH_MAX];
  const std::string pattern("/tmp_XXXXXX");
  const std::string parent_str(parent_dir.string());
  std::strncpy(tmpdir, parent_str.c_str(), parent_str.size() + 1);
  std::strncat(tmpdir, pattern.c_str(), pattern.size() + 1);
  if (mkdtemp(tmpdir) == nullptr) {
    signal_error("mkdtemp failed");
  }
  return tmpdir;
}

inline void CreateOrCheckIfEmpty(const fs::path& directory) {
  if (fs::is_directory(directory)) {
    if (!fs::is_empty(directory)) {
      signal_error("is not empty", directory);
    }
  } else {
    fs::create_directories(directory);
  }
}

} // namespace netspeak::util

#endif // NETSPEAK_UTIL_SYSTEMIO_HPP
