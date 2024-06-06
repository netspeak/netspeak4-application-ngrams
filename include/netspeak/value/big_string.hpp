// big_string.hpp -*- C++ -*-
// Copyright (C) 2013 Martin Trenkmann
#ifndef NETSPEAK_VALUE_BIG_STRING_HPP
#define NETSPEAK_VALUE_BIG_STRING_HPP

/** \todo I don't see, why big_string.hpp and big_string_traits.hpp exist. Deprecated? **/

#include <string>

namespace netspeak::value {

struct big_string {
  big_string() = default;

  explicit big_string(const char* other) : str(other) {}

  big_string(const big_string& other) : str(other.str) {}

  std::string str;
};

inline bool operator==(const big_string& lhs, const big_string& rhs) {
  return lhs.str == rhs.str;
}

inline bool operator!=(const big_string& lhs, const big_string& rhs) {
  return !(lhs == rhs);
}

inline bool operator<(const big_string& lhs, const big_string& rhs) {
  return lhs.str < rhs.str;
}

inline bool operator>(const big_string& lhs, const big_string& rhs) {
  return lhs.str > rhs.str;
}

inline std::ostream& operator<<(std::ostream& os, const big_string& value) {
  return os << value.str;
}

} // namespace netspeak::value

#endif // NETSPEAK_VALUE_BIG_STRING_HPP
