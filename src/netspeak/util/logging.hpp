// logging.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_UTIL_LOGGING_HPP
#define NETSPEAK_UTIL_LOGGING_HPP

#include <iostream>
#include <iterator>
#include <mutex>
#include <string>

/**
 * Functions for logging output.
 */
namespace netspeak {
namespace util {

std::mutex& get_log_mutex();

std::ostream& print_timestamp_to(std::ostream& os);

void log(const std::string& msg);
void log(const char* file, int line, const char* msg);

template <typename T>
void log(const std::string& msg, const T& value) {
  std::lock_guard<std::mutex> guard(get_log_mutex());
  print_timestamp_to(std::cout) << ' ' << msg << ' ' << value << std::endl;
}

template <typename T>
void log(const char* file, int line, const char* msg, const T& value) {
  std::lock_guard<std::mutex> guard(get_log_mutex());
  print_timestamp_to(std::cout)
      << ' ' << file << ':' << line << ' ' << msg << ' ' << value << std::endl;
}

template <typename Iter>
void log(const char* file, int line, const char* msg, Iter first, Iter last) {
  std::lock_guard<std::mutex> guard(get_log_mutex());
  print_timestamp_to(std::cout)
      << ' ' << file << ':' << line << ' ' << msg << std::endl;
  std::copy(first, last,
            std::ostream_iterator<typename Iter::value_type>(std::cout, "\n"));
}

} // namespace util
} // namespace netspeak

#ifdef DEBUG_OUTPUT_ENABLED
#define DEBUG_LOG(args...) util::log(__FILE__, __LINE__, args)
#else
#define DEBUG_LOG(args...) ((void)0)
#endif // DEBUG_OUTPUT_ENABLED

#endif // NETSPEAK_UTIL_LOGGING_HPP
