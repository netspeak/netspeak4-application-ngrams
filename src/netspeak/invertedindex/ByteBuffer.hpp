// ByteBuffer.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_BYTE_BUFFER_HPP
#define NETSPEAK_INVERTEDINDEX_BYTE_BUFFER_HPP

#include <memory>
#include <cstdlib>
#include <cstring>

#include "netspeak/util/systemio.hpp"
#include "netspeak/util/exception.hpp"
#include "netspeak/value/big_string_traits.hpp"
#include "netspeak/value/pair_traits.hpp"
#include "netspeak/value/quadruple_traits.hpp"
#include "netspeak/value/quintuple_traits.hpp"
#include "netspeak/value/sextuple_traits.hpp"
#include "netspeak/value/string_traits.hpp"
#include "netspeak/value/triple_traits.hpp"

namespace netspeak {
namespace invertedindex {

/**
 * A class to represent a raw byte buffer to store primitives and strings.
 * Note that the class' assignment operator returns a shallow copy of its
 * backed array which is managed by a smart pointer. Use the clone() method
 * to get a new deep copy of the data.
 */
class ByteBuffer {
private:
  struct buffer_type {
    buffer_type(size_t initial_size)
        : data(new char[initial_size]), size(initial_size) {
      if (!data)
        throw std::bad_alloc();
    }
    void resize(size_t new_size) {
      if (new_size != size && new_size != 0) {
        data.reset(new char[new_size]);
        if (!data)
          throw std::bad_alloc();
      }
      size = new_size;
    }
    std::unique_ptr<char[]> data;
    size_t size;
  };

public:
  /**
   * The explicit constructor.
   * The byte buffer has a initial size of <code>size</code>.
   */
  ByteBuffer(size_t size = 0)
      : buffer_(new buffer_type(size)), bufpos_(buffer_->data.get()) {}

  /**
   * The copy constructor for shallow copying.
   * @param buffer
   */
  ByteBuffer(const ByteBuffer& buffer)
      : buffer_(buffer.buffer_), bufpos_(buffer_->data.get()) {}

  /**
   * The destructor.
   */
  ~ByteBuffer() {}

private:
  inline size_t bytes_available() const {
    return buffer_->size - (bufpos_ - buffer_->data.get());
  }

public: // methods
  char* begin() { return buffer_->data.get(); }

  const char* begin() const { return buffer_->data.get(); }

  void clear() {
    // does not free memory
    buffer_->size = 0;
    rewind();
  }

  /**
   * Creates a deep copy of that buffer.
   */
  ByteBuffer clone() const {
    ByteBuffer deep_copy(size());
    std::memcpy(deep_copy.buffer_->data.get(), begin(), size());
    return deep_copy;
  }

  char* end() { return buffer_->data.get() + buffer_->size; }

  const char* end() const { return buffer_->data.get() + buffer_->size; }

  /**
   * The assignment operator for shallow copying.
   * The internal position will be set to the beginning.
   * @param buffer
   */
  ByteBuffer& operator=(const ByteBuffer& buffer) {
    buffer_ = buffer.buffer_;
    bufpos_ = buffer_->data.get();
    return *this;
  }

  template <typename T> bool get(T& value) {
    typedef value::value_traits<T> traits_type;
    if (!bufpos_ || bufpos_ + sizeof(value) > buffer_->data.get() + buffer_->size)
      return false;
    const char* new_bufpos(traits_type::copy_from(value, bufpos_));
    bufpos_ = const_cast<char*>(new_bufpos);
    return true;
  }

  template <typename T> bool put(const T& value) {
    typedef value::value_traits<T> traits_type;
    if (traits_type::size_of(value) > bytes_available())
      return false;
    bufpos_ = traits_type::copy_to(value, bufpos_);
    return true;
  }

  void seek(size_t offset) {
    if (offset > size()) {
      util::throw_out_of_range("ByteBuffer::seek failed", offset);
    }
    bufpos_ = buffer_->data.get() + offset;
  }

  size_t tell() const { return bufpos_ - begin(); }

  void rewind() { bufpos_ = buffer_->data.get(); }

  void read(FILE* fs) {
    util::fread(buffer_->data.get(), 1, size(), fs);
    rewind();
  }

  void write(FILE* fs) const { util::fwrite(begin(), 1, size(), fs); }

  /**
   * Allocates <tt>size</tt> bytes of memory.
   * @param size the new size of the byte buffer.
   */
  void resize(size_t size) {
    buffer_->resize(size);
    rewind();
  }

  /**
   * Get the size of the byte array.
   * @return the number of bytes stored.
   */
  size_t size() const { return buffer_->size; }

  const char* position() const { return bufpos_; }

private:
  std::shared_ptr<buffer_type> buffer_;
  char* bufpos_;
};

} // namespace invertedindex
} // namespace netspeak

#endif // NETSPEAK_INVERTEDINDEX_BYTE_BUFFER_HPP
