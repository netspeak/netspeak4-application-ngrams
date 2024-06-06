// ManagedIndexer.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_MANAGED_INDEXER_HPP
#define NETSPEAK_INVERTEDINDEX_MANAGED_INDEXER_HPP

#include <format>
#include <fstream>
#include <stdexcept>

#include "Indexer.hpp"
#include "InvertedFileReader.hpp"
#include "Properties.hpp"
#include "Record.hpp"

namespace netspeak::invertedindex {

namespace fs = std::filesystem;

/**
 * A class to create an inverted index from input files.
 */
template <typename T, typename Reader = InvertedFileReader<T> >
class ManagedIndexer {
public:
  typedef Record<T> RecordType;
  typedef Indexer<T> IndexerType;

  static const Properties index(const Configuration& config) {
    uint64_t total_input_size = 0;
    uint64_t current_input_size = 0;
    uint64_t current_record_count = 0;

    const auto read_file = [&](const fs::path& file, IndexerType& indexer) {
      if (fs::is_regular_file(file) && !util::is_hidden_file(file)) {
        std::ifstream ifs(file);
        if (!ifs) {
          throw std::invalid_argument(std::format("Cannot open : {}", file));
        }
        RecordType record;
        Reader reader(ifs);
        util::log("Processing", file);
        while (reader.read(record)) {
          try {
            indexer.insert(record);
            if (!(++current_record_count % 10000000)) {
              // Estimate the total number of records.
              const double avg_record_size =
                  (current_input_size + reader.tell()) / static_cast<double>(current_record_count);
              const uint64_t total_record_count = total_input_size / avg_record_size;
              indexer.set_expected_record_count(total_record_count);
            }
          } catch (std::exception& e) {
            util::log("Indexer::insert failed", e.what());
          }
        }
        current_input_size += reader.tell();
      }
    };

    IndexerType indexer(config);
    if (!config.input_file().empty() && !config.input_directory().empty()) {
      throw std::invalid_argument("Specify either an input file or a directory, not both");
    }
    if (!config.input_file().empty()) {
      const fs::path input_file = config.input_file();
      if (!fs::exists(input_file)) {
        throw std::runtime_error(std::format("Does not exist : {}", input_file));
      }
      if (!fs::is_regular_file(input_file)) {
        throw std::runtime_error(std::format("No regular file : {}", input_file));
      }
      total_input_size = fs::file_size(input_file);
      read_file(input_file, indexer);
    } else if (!config.input_directory().empty()) {
      const fs::path input_dir = config.input_directory();
      if (!fs::exists(input_dir)) {
        throw std::runtime_error(std::format("Does not exist : {}", input_dir));
      }
      if (!fs::is_directory(input_dir)) {
        throw std::runtime_error(std::format("Not a directory : {}", input_dir));
      }
      total_input_size = util::directory_size(input_dir);
      const fs::directory_iterator end;
      for (fs::directory_iterator it(input_dir); it != end; ++it) {
        read_file(it->path(), indexer);
      }
    } else {
      throw std::invalid_argument("No input file or directory specified");
    }
    return indexer.index();
  }
};

} // namespace netspeak::invertedindex

#endif // NETSPEAK_INVERTEDINDEX_MANAGED_INDEXER_HPP
