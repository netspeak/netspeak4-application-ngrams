#ifndef NETSPEAK_ERROR_HPP
#define NETSPEAK_ERROR_HPP

#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace netspeak {


struct invalid_query : public std::logic_error {
  invalid_query(const std::string& query);
  invalid_query(size_t startLine, size_t startPoint, size_t endLine,
                size_t endPoint, std::string msg);

  virtual ~invalid_query() throw();
};


/**
 * @brief The query_error_message struct
 *
 * returns a standard error messages for an invalid query
 */
struct query_error_message {
  static const std::string too_many_words;
  static const std::string too_many_regex_in_orderset;
  static const std::string invalid_regexword;
  static const std::string too_complex(uint32_t complexity,
                                       uint32_t max_worst_case_complexity);
  static const std::string too_long_query(int queryLength, int queryMaxLength);
};

struct error_message {
  static const std::string no_error;
  static const std::string invalid_query;
  static const std::string server_error;
  static const std::string unknown_error;
  // messages with no corresponding error code
  static const std::string cannot_create;
  static const std::string cannot_open;
  static const std::string does_not_exist;
  static const std::string is_not_empty;
};

/**
 * Defines error codes used by the generated Protobuf \c Response object.
 * For compatibility reasons we use no enum types to encode error codes.
 */
enum class error_code {
  no_error = 0,
  invalid_query = 1,
  server_error = 2,
  unknown_error = 3,
  size,
};


error_code to_error_code(uint32_t ec);

uint32_t to_ordinal(error_code ec);

const std::string& to_string(error_code ec);


std::ostream& operator<<(std::ostream& os, error_code ec);

std::string sizetToString(size_t number);

} // namespace netspeak

#endif // NETSPEAK_ERROR_HPP
