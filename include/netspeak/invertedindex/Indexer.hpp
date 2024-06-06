// Indexer.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_INDEXER_HPP
#define NETSPEAK_INVERTEDINDEX_INDEXER_HPP

#include <cstring>
#include <format>
#include <memory>
#include <ostream>
#include <stdexcept>

#include "Configuration.hpp"
#include "IndexStrategy.hpp"
#include "RawIndexer.hpp"
#include "SortedInput.hpp"
#include "UnsortedInput.hpp"

namespace netspeak::invertedindex {

/**
 * A class to create an inverted index from a number of records.
 */
template <typename T>
class Indexer : public RawIndexer {
public:
  typedef T value_type;
  typedef typename IndexStrategy<value_type>::record_type record_type;

  explicit Indexer(const Configuration& config) {
    assert_config(config);
    util::log("Configuration", config);
    strategy_ = config.key_sorting() == key_sorting_type::sorted
                    ? std::unique_ptr<IndexStrategy<typename record_type::value_type> >(
                          new SortedInput<typename record_type::value_type>(config))
                    : std::unique_ptr<IndexStrategy<typename record_type::value_type> >(
                          new UnsortedInput<typename record_type::value_type>(config));
  }

  virtual ~Indexer() {}

  const Properties index() override {
    Properties props;
    strategy_->index();
    props.key_count = strategy_->stats().key_count;
    props.value_count = strategy_->stats().value_count;
    props.total_size = strategy_->stats().total_size;
    props.value_sorting = strategy_->config().value_sorting();
    const std::string tn(value::value_traits<typename record_type::value_type>::type_name());
    /** \todo Potentially unchecked memory issue here since this may not write a \0, if tn is too long! **/
    std::strncpy(props.value_type, tn.c_str(), sizeof(props.value_type));
    props.write(fs::path(strategy_->config().index_directory()) / Properties::default_filename());
    util::log("Indexing succeeded");
    util::log("Properties", props);
    return props;
  }

  void insert(const record_type& record) {
    assert_record(record);
    strategy_->insert(record);
    if (!(strategy_->stats().value_count % 10000000)) {
      util::log("#records inserted", strategy_->stats().value_count);
    }
  }

  void insert(const std::string& key, const value_type& value) {
    insert(record_type(key, value));
  }

  void insert(const std::string& key, const char* value) {
    record_type record(key);
    value::value_traits<typename record_type::value_type>::copy_from(record.value(), value);
    insert(record);
  }

  void set_expected_record_count(uint64_t record_count) {
    strategy_->set_expected_record_count(record_count);
  }

private: // class member
  static void assert_config(const Configuration& config) {
    if (!fs::exists(config.index_directory())) {
      throw std::invalid_argument(std::format("Does not exist : {}", config.index_directory()));
    }
    if (!fs::is_directory(config.index_directory())) {
      throw std::invalid_argument(std::format("Not a directory : {}", config.index_directory()));
    }
    if (!fs::is_empty(config.index_directory())) {
      throw std::invalid_argument(std::format("Is not empty : {}", config.index_directory()));
    }
  }

  static void assert_record(const record_type& record) {
    typedef value::value_traits<typename record_type::key_type> ktraits_t;
    typedef value::value_traits<typename record_type::value_type> vtraits_t;

    if (record.key().size() > std::numeric_limits<typename ktraits_t::io_size_type>::max()) {
      throw std::invalid_argument(std::format("Too long key : {}", record));
    }
    if (vtraits_t::size_of(record.value()) > std::numeric_limits<typename vtraits_t::io_size_type>::max()) {
      throw std::invalid_argument(std::format("Too long value : {}", record));
    }
  }

private:
  std::unique_ptr<IndexStrategy<typename record_type::value_type> > strategy_;
};

} // namespace netspeak::invertedindex

#endif // NETSPEAK_INVERTEDINDEX_INDEXER_HPP
