// RecordReader.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_RECORD_READER_HPP
#define NETSPEAK_INVERTEDINDEX_RECORD_READER_HPP

#include "Record.hpp"

namespace netspeak::invertedindex {

template <typename T>
class RecordReader {
public:
  typedef Record<T> record_type;

  RecordReader() = default;
  virtual ~RecordReader() = default;

  virtual bool read(record_type& record) = 0;

  virtual std::size_t tell() = 0;
};

} // namespace netspeak::invertedindex

#endif // NETSPEAK_INVERTEDINDEX_RECORD_READER_HPP
