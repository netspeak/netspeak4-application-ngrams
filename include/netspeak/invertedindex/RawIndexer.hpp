// RawIndexer.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_RAW_INDEXER_HPP
#define NETSPEAK_INVERTEDINDEX_RAW_INDEXER_HPP

#include <inttypes.h>

#include <string>

#include "Properties.hpp"

namespace netspeak::invertedindex {

/**
 * A class to create an inverted index from a number of records.
 */
class RawIndexer {
public:
  virtual ~RawIndexer() = default;

  virtual const Properties index() = 0;

  virtual void insert(const std::string& key, const char* value) = 0;

  virtual void set_expected_record_count(uint64_t record_count) = 0;
};

} // namespace netspeak::invertedindex

#endif // NETSPEAK_INVERTEDINDEX_RAW_INDEXER_HPP
