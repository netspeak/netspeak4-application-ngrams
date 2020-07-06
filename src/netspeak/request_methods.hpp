#ifndef NETSPEAK_REQUEST_METHODS_HPP
#define NETSPEAK_REQUEST_METHODS_HPP

//#include "netspeak/generated/NetspeakMessages.pb.h"

namespace netspeak {
namespace generated {
// Operators must be defined in the same namespace as its operands for
// argument-dependent lookup (ADL, also known as Koenig lookup).

// Semantic:
//  If true, the retrieved result set of 'a' must be a superset of the result
//  set retrieved by 'b'. This condition cannot be satisfied just checking if
//  each parameter is >= (or <= for phrase_length_min).
//  For instance, if a.max_phrase_frequency > b.max_phrase_frequency and
//  a.max_phrase_count == b.max_phrase_count, then the result of 'a' might
//  not include the entire result of 'b'. Therefore we have to compare
//  max_phrase_frequency for equality.
/*bool operator>=(const Request& a, const Request& b);*/

/*bool operator==(const Request& a, const Request& b);*/

} // namespace generated

bool is_compatible(const generated::Request& a, const generated::Request& b);

} // namespace netspeak

#endif // NETSPEAK_REQUEST_METHODS_HPP
