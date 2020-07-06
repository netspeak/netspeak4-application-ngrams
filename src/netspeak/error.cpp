#include "netspeak/error.hpp"

#include <string>

namespace netspeak {

const std::string query_error_message::too_many_words(
    "There are too many words in this section");
const std::string query_error_message::too_many_regex_in_orderset(
    "There are too many regexwords in this orderset");
const std::string query_error_message::invalid_regexword("invalid regexword");
const std::string query_error_message::too_deeply_nested(
    "The query is too deeply nested");
const std::string query_error_message::too_complex(
    uint32_t complexity, uint32_t max_worst_case_complexity) {
  std::stringstream error;
  error << "This query is too complex. Your complexity is ";
  error << complexity;
  error << " but the maximum complexity is ";
  error << max_worst_case_complexity;
  error << ".";

  return error.str();
}
const std::string query_error_message::too_long_query(int queryLength,
                                                      int queryMaxLength) {
  std::stringstream error;
  error << "This query is too long. query length : ";
  error << queryLength;
  error << " max query length : ";
  error << queryMaxLength;
  return error.str();
}

const std::string error_message::no_error("no error");

const std::string error_message::invalid_query("invalid query");

const std::string error_message::server_error("server error");

const std::string error_message::unknown_error("unknown error");

const std::string error_message::cannot_create("cannot create");

const std::string error_message::cannot_open("cannot open");

const std::string error_message::does_not_exist("does not exist");

const std::string error_message::is_not_empty("is not empty");

error_code to_error_code(uint32_t ec) {
  return ec < to_ordinal(error_code::size) ? static_cast<error_code>(ec)
                                           : error_code::unknown_error;
}

uint32_t to_ordinal(error_code ec) {
  return static_cast<uint32_t>(ec);
}

const std::string& to_string(error_code ec) {
  switch (ec) {
    case error_code::no_error:
      return error_message::no_error;
    case error_code::invalid_query:
      return error_message::invalid_query;
    case error_code::server_error:
      return error_message::server_error;
    default:
      return error_message::unknown_error;
  }
}


std::ostream& operator<<(std::ostream& os, error_code ec) {
  return os << to_string(ec);
}

std::string sizetToString(size_t sz) {
  std::stringstream stream;

  stream << sz;

  return stream.str();
}

} // namespace netspeak
