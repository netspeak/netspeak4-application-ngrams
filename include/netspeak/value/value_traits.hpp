// value_traits.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_VALUE_VALUE_TRAITS_HPP
#define NETSPEAK_VALUE_VALUE_TRAITS_HPP

#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace netspeak::value {

// -----------------------------------------------------------------------------
// Template specializations to obtain the name of some primitive type.
// -----------------------------------------------------------------------------

template <typename T>
struct NameOf;

template <>
struct NameOf<int8_t> {
  constexpr static std::string value = "Byte";
};

template <>
struct NameOf<uint8_t> {
  constexpr static std::string value = "UByte";
};

template <>
struct NameOf<int16_t> {
  constexpr static std::string value = "Short";
};

template <>
struct NameOf<uint16_t> {
  constexpr static std::string value = "UShort";
};

template <>
struct NameOf<int32_t> {
  constexpr static std::string value = "Int";
};

template <>
struct NameOf<uint32_t> {
  constexpr static std::string value = "UInt";
};

template <>
struct NameOf<int64_t> {
  constexpr static std::string value = "Long";
};

template <>
struct NameOf<uint64_t> {
  constexpr static std::string value = "ULong";
};

template <>
struct NameOf<float> {
  constexpr static std::string value = "Float";
};

template <>
struct NameOf<double> {
  constexpr static std::string value = "Double";
};

// -----------------------------------------------------------------------------
// Primary template for arithmetic types (boolean, int, float, double)
// -----------------------------------------------------------------------------

template <typename T>
inline void print_to_os(T value, std::ostream& os) {
  os << value;
}

// It's safer to overload a function, rather than specializing the template.
// (http://www.gotw.ca/publications/mill17.htm)
inline void print_to_os(int8_t value, std::ostream& os) {
  os << static_cast<int16_t>(value);
}

// It's safer to overload a function, rather than specializing the template.
// (http://www.gotw.ca/publications/mill17.htm)
inline void print_to_os(uint8_t value, std::ostream& os) {
  os << static_cast<uint16_t>(value);
}

static constexpr char tuple_element_separator = '\t';

template <typename T>
struct value_traits {
  typedef typename std::enable_if<std::is_arithmetic<T>::value, T>::type value_type;
  typedef uint16_t io_size_type; // not needed for types with constant size

  static constexpr std::size_t ByteSize() {
    return sizeof(T);
  }

  static inline size_t size_of(const value_type& value) {
    return sizeof(value);
  }

  static inline char* copy_to(const value_type& value, char* buffer) {
    std::memcpy(buffer, &value, sizeof(value));
    return buffer + sizeof(value);
  }

  static inline const char* copy_from(value_type& value, const char* buffer) {
    std::memcpy(&value, buffer, sizeof(value));
    return buffer + sizeof(value);
  }

  static inline bool write_to(const value_type& value, FILE* file) {
    return std::fwrite(&value, sizeof(value), 1, file) == 1;
  }

  static inline bool read_from(value_type& value, FILE* file) {
    return std::fread(&value, sizeof(value), 1, file) == 1;
  }

  static inline void print_to(const value_type& value, std::ostream& os) {
    print_to_os(value, os);
  }

  static inline void println_to(const value_type& value, std::ostream& os) {
    print_to(value, os);
    os << '\n';
  }

  /**
   * Parses and sets new data for the given value from the given stream.
   * The routine also extracts the {@link tuple_element_separator} if any.
   */
  static inline void parse_from(value_type& value, std::istream& is) {
    is >> value;
    if (is.fail()) {
      throw std::runtime_error(std::format("Reading next {} from input stream failed.", type_name()));
    }
    if (is.peek() == tuple_element_separator)
      is.get();
  }

  static inline std::string type_name() {
    return NameOf<value_type>::value;
  }
};

// -----------------------------------------------------------------------------
// Random and numbered instance generators needed for unit testing.
// -----------------------------------------------------------------------------

template <typename T>
struct generator {
  typedef T value_type;

  static void randomized(value_type& value) {
    value = ::rand();
  }

  static void numbered(value_type& value, size_t number) {
    value = number;
  }
};

} // namespace netspeak::value

#endif // NETSPEAK_VALUE_VALUE_TRAITS_HPP
