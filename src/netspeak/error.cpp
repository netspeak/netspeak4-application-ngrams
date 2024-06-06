#include <format>
#include <netspeak/error.hpp>
#include <string>

namespace netspeak {

namespace query_error_message {
const std::string too_complex(uint32_t complexity, uint32_t max_worst_case_complexity) {
  return std::format("This query is too complex. Your complexity is {} but the maximum complexity is {}.", complexity,
                     max_worst_case_complexity);
}
const std::string too_long_query(int queryLength, int queryMaxLength) {
  return std::format("This query is too long. query length : {} max query length : ", queryLength, queryMaxLength);
}
} // namespace query_error_message
} // namespace netspeak
