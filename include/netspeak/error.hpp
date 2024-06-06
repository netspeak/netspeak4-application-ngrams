#ifndef NETSPEAK_ERROR_HPP
#define NETSPEAK_ERROR_HPP

#include <inttypes.h>

#include <string>

#include "util/traceable_error.hpp"

namespace netspeak {

using util::tracable_logic_error;
using util::tracable_runtime_error;

struct invalid_query_error final : public tracable_runtime_error {
  explicit invalid_query_error(const std::string& what) : tracable_runtime_error(what) {}
  virtual ~invalid_query_error() override = default;
};


/**
 * @brief Contains standard error messages for an invalid query
 */
namespace query_error_message {
inline const std::string too_many_words("There are too many words in this section");
inline const std::string too_many_regex_in_orderset("There are too many regexwords in this orderset");
inline const std::string invalid_regexword("invalid regexword");
inline const std::string too_deeply_nested("The query is too deeply nested");
extern const std::string too_complex(uint32_t complexity, uint32_t max_worst_case_complexity);
extern const std::string too_long_query(int queryLength, int queryMaxLength);
}; // namespace query_error_message

namespace error_message {
inline const std::string no_error("no error");
inline const std::string invalid_query("invalid query");
inline const std::string server_error("server error");
inline const std::string unknown_error("unknown error");
// messages with no corresponding error code
inline const std::string cannot_create("cannot create");
inline const std::string cannot_open("cannot open");
inline const std::string does_not_exist("does not exist");
inline const std::string is_not_empty("is not empty");
}; // namespace error_message

} // namespace netspeak

#endif