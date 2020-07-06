#include "netspeak/request_methods.hpp"

namespace netspeak {
namespace generated {
/*
bool operator>=(const Request& a, const Request& b) {
  return (a.query() == b.query()) &&
         (a.max_phrase_frequency() == b.max_phrase_frequency()) &&
         (a.max_phrase_count() >= b.max_phrase_count()) &&
         (a.phrase_length_min() <= b.phrase_length_min()) &&
         (a.phrase_length_max() >= b.phrase_length_max()) &&
         (a.pruning_low() >= b.pruning_low()) &&
         (a.pruning_high() >= b.pruning_high());
}

bool operator==(const Request& a, const Request& b) {
  return (a.query() == b.query()) &&
         (a.max_phrase_frequency() == b.max_phrase_frequency()) &&
         (a.max_phrase_count() == b.max_phrase_count()) &&
         (a.phrase_length_min() == b.phrase_length_min()) &&
         (a.phrase_length_max() == b.phrase_length_max()) &&
         (a.pruning_low() == b.pruning_low()) &&
         (a.pruning_high() == b.pruning_high());
}*/

} // namespace generated

bool is_compatible(const generated::Request& a, const generated::Request& b) {
  return (a.query() == b.query()) &&
         (a.phrase_length_min() == b.phrase_length_min()) &&
         (a.phrase_length_max() == b.phrase_length_max()) &&
         (a.pruning_low() == b.pruning_low()) &&
         (a.pruning_high() == b.pruning_high());
}

} // namespace netspeak
