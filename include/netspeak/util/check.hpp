// check.hpp -*- C++ -*-
// Copyright (C) 2008-2013 Martin Trenkmann
#ifndef NETSPEAK_UTIL_CHECK_HPP
#define NETSPEAK_UTIL_CHECK_HPP

#include <format>
#include <netspeak/util/std_path_formatter.hpp>
#include <stdexcept>
#include <string>

namespace netspeak::util {

inline void check(bool expr, const std::string& msg) {
  if (!expr) {
    throw std::runtime_error(msg);
  }
}

template <typename T>
void check(bool expr, const std::string& msg, const T& obj) {
  if (!expr) {
    throw std::runtime_error(std::format("{} : {}", msg, obj));
  }
}

template <typename T>
void check(bool expr, const std::string& func, const std::string& msg, const T& obj) {
  if (!expr) {
    throw std::runtime_error(std::format("{} : {} : {}", func, msg, obj));
  }
}

template <typename T>
void check_equal(const T& expected, const T& actual, const std::string& message = "") {
  if (!(expected == actual)) {
    throw std::runtime_error(std::format("{}\nexpected: {}\nactual: ", message, expected, actual));
  }
}

} // namespace netspeak::util

#endif // NETSPEAK_UTIL_CHECK_HPP
