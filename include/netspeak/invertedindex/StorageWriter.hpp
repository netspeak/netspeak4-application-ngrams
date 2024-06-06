// StorageWriter.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_STORAGE_WRITER_HPP
#define NETSPEAK_INVERTEDINDEX_STORAGE_WRITER_HPP

#include <format>
#include <stdexcept>

#include <boost/utility.hpp>

#include "../bighashmap/Builder.hpp"
#include "../util/conversion.hpp"
#include "Postlist.hpp"
#include "Record.hpp"

namespace netspeak::invertedindex {

namespace fs = std::filesystem;

template <typename T>
class StorageWriter : public boost::noncopyable {
public:
  /**
   * The header table's value type.
   */
  typedef value::pair<uint16_t, uint32_t> Address;

  static const std::string k_data_dir;
  static const std::string k_data_file;
  static const std::string k_table_dir;
  static const std::string k_table_file;

  static const size_t data_file_size_max = 1024 * 1024 * 1024; // 1GB

  explicit StorageWriter(const fs::path& directory) : directory_(directory), data_file_cnt_(), data_wfs_(nullptr) {
    if (!fs::exists(directory)) {
      throw std::invalid_argument(std::format("Does not exist : {}", directory));
    }
    if (!fs::is_directory(directory)) {
      throw std::invalid_argument(std::format("Not a directory : {}", directory));
    }
    //    if (!fs::is_empty(directory)) {
    //      throw std::invalid_argument(std::format("Is not empty : {}", directory));
    //    }
  }

  ~StorageWriter() {
    close();
  }

  void close() {
    if (data_wfs_ == nullptr)
      return;

    table_ofs_.close();
    util::fclose(data_wfs_);
    data_wfs_ = nullptr;
    const fs::path table_dir(directory_ / k_table_dir);
    if (!fs::create_directory(table_dir)) {
      throw std::invalid_argument(std::format("Cannot create : {}", table_dir));
    }
    util::log("Building header table");
    bighashmap::Builder<Address>::Build(directory_, table_dir);
  }

  bool write(const std::string& key, Postlist<T>& postlist) {
    if (postlist.empty()) {
      return false;
    }
    if (key.empty()) {
      throw std::invalid_argument("Cannot write empty key");
    }
    const size_t postlist_size(postlist.byte_size());
    if (data_wfs_ == nullptr) {
      const fs::path data_dir(directory_ / k_data_dir);
      if (!fs::create_directory(data_dir)) {
        throw std::invalid_argument(std::format("Cannot create : {}", data_dir));
      }
      const std::string num(util::to_string(data_file_cnt_++));
      data_wfs_ = util::fopen(data_dir / (k_data_file + num), "wb");
      const fs::path table_file(directory_ / k_table_file);
      table_ofs_.open(table_file);
      if (!table_ofs_) {
        throw std::invalid_argument(std::format("Cannot create : {}", table_file));
      }
    } else if (util::ftell(data_wfs_) + postlist_size > data_file_size_max) {
      util::fclose(data_wfs_);
      const fs::path data_dir(directory_ / k_data_dir);
      const std::string num(util::to_string(data_file_cnt_++));
      data_wfs_ = util::fopen(data_dir / (k_data_file + num), "wb");
    }
    Record<Address> record(key, Address(data_file_cnt_ - 1, util::ftell(data_wfs_)));
    postlist.write(data_wfs_);
    return (table_ofs_ << record << '\n').good();
  }

private:
  fs::path directory_;
  std::ofstream table_ofs_;
  uint16_t data_file_cnt_;
  FILE* data_wfs_;
};

template <typename T>
const std::string StorageWriter<T>::k_data_dir("data");

template <typename T>
const std::string StorageWriter<T>::k_data_file("data.");

template <typename T>
const std::string StorageWriter<T>::k_table_dir("table");

template <typename T>
const std::string StorageWriter<T>::k_table_file("table.txt");

} // namespace netspeak::invertedindex

#endif // NETSPEAK_INVERTEDINDEX_STORAGE_WRITER_HPP
