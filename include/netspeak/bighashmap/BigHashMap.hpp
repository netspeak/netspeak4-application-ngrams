// BigHashMap.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_BIGHASHMAP_BIG_HASH_MAP_HPP
#define NETSPEAK_BIGHASHMAP_BIG_HASH_MAP_HPP

#include <fstream>
#include <netspeak/util/std_path_formatter.hpp>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "../util/memory.hpp"
#include "../value/pair_traits.hpp"
#include "../value/quadruple_traits.hpp"
#include "../value/triple_traits.hpp"
#include "Builder.hpp"
#include "CmphMap.hpp"
#include "ExternalMap.hpp"
#include "InternalMap.hpp"

namespace netspeak::bighashmap {

/**
 * For comments please take a look at {@link ExternalMap}.
 * By default read access to the hash table is not thread-safe.
 *
 * @author  martin.trenkmann@uni-weimar.de
 * @version $Id$
 */
template <typename ValueT, bool IsThreadSafe = false, typename TraitsT = value::value_traits<ValueT> >
class BigHashMap final {
public:
  typedef ValueT Value;
  typedef TraitsT Traits;

  BigHashMap() = delete;

  ~BigHashMap() {
    /** \todo use unique_ptr instead of raw pointers to encode this onwership semantic and provide automatic
     * deallocation. **/
    for (const auto& map : maps_) {
      delete map;
    }
  }

private:
  typedef CmphMap<ValueT, Traits> Map;

  static const std::string index_file_name;

  explicit BigHashMap(const std::vector<Map*>& sub_maps) : maps_(sub_maps), size_(0) {
    for (const auto& map : maps_) {
      size_ += map->size();
    }
  }

  static uint64_t SerializedSizeInBytes(const fs::path& dir) {
    uint64_t size = 0;
    if (fs::exists(dir)) {
      /** \todo Refactor **/
      const fs::directory_iterator end;
      for (fs::directory_iterator it(dir); it != end; ++it) {
        if (!fs::is_regular_file(it->path()))
          continue;
        if (boost::ends_with(it->path().string(), "dat") || boost::ends_with(it->path().string(), "mph")) {
          size += fs::file_size(it->path());
        }
      }
    }
    return size;
  }

public:
  static void Build(const fs::path& input_dir, const fs::path& output_dir, Algorithm algorithm = Algorithm::BDZ) {
    // Set current locale to C to enable byte-wise UTF-8 string comparison.
    // http://stackoverflow.com/questions/20226851/how-do-locales-work-in-linux-posix-and-what-transformations-are-applied/20231523
    std::locale::global(std::locale("C"));
    bighashmap::Builder<Value>::Build(input_dir, output_dir, algorithm);
  }

  static BigHashMap* Open(const fs::path& dir, util::memory_type memory = util::memory_type::min_required) {
    const fs::path idx_file = dir / index_file_name;
    std::ifstream ifs(idx_file);
    if (!ifs) {
      throw std::runtime_error(std::format("Cannot open : {}", idx_file));
    }
    std::string part_idx_file;
    std::vector<Map*> part_maps;
    const uint64_t num_bytes = SerializedSizeInBytes(idx_file.parent_path());
    if (num_bytes > util::to_bytes(memory)) {
      while (std::getline(ifs, part_idx_file)) {
        part_maps.push_back(ExternalMap<ValueT, IsThreadSafe, Traits>::Open(idx_file.parent_path() / part_idx_file));
      }
    } else {
      while (std::getline(ifs, part_idx_file)) {
        part_maps.push_back(InternalMap<ValueT, Traits>::Open(idx_file.parent_path() / part_idx_file));
      }
    }
    ifs.close();
    return new BigHashMap(part_maps);
  }

  bool Get(const std::string& key, Value& value) {
    return maps_.empty() ? false : maps_[util::hash32(key) % maps_.size()]->Get(key, value);
  }

  bool Get(const std::string& key, char* buffer) {
    Value value;
    if (Get(key, value)) {
      Traits::copy_to(value, buffer);
      return true;
    }
    return false;
  }

  uint64_t size() const {
    return size_;
  }

  Algorithm algorithm() const {
    return maps_.empty() ? Algorithm::None : maps_.front()->algorithm();
  }

private:
  const std::vector<Map*> maps_;
  uint64_t size_; /** \todo: with slight modification of the constructor, size_ can be const **/
};

template <typename Value, bool IsThreadSafe, typename Traits>
const std::string BigHashMap<Value, IsThreadSafe, Traits>::index_file_name(bighashmap::Builder<Value>::index_file_name);

} // namespace netspeak::bighashmap

#endif // NETSPEAK_BIGHASHMAP_BIG_HASH_MAP_HPP
