#ifndef NETSPEAK_REGEX_PARSERS_HPP
#define NETSPEAK_REGEX_PARSERS_HPP

#include <string>

#include "netspeak/regex/RegexQuery.hpp"


namespace netspeak {
namespace regex {

/**
 * @brief Parses the given Netspeak regex query and returns the equivalent
 * general regex query.
 *
 * Note: The parser guarantees that every string will be parsed as a query.
 * Every syntactically incorrect part of a query will be interpreted as a
 * literal word. E.g. `fo[o` will be parsed as the word "fo[o" instead of not at
 * all because of the unclosed bracket.
 *
 * @param netspeak_query
 * @return RegexQuery
 */
RegexQuery parse_netspeak_regex_query(const std::string& netspeak_query);

} // namespace regex
} // namespace netspeak

#endif // NETSPEAK_REGEX_PARSERS_HPP
