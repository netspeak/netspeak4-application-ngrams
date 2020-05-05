#ifndef NETSPEAK_QUERY_METHODS_HPP
#define NETSPEAK_QUERY_METHODS_HPP

#include <ostream>
#include <string>

#include "netspeak/generated/NetspeakMessages.pb.h"

namespace netspeak {

struct wildcard {
  static const char hash = '#';
  static const char qmark = '?';
  static const char asterisk = '*';
  static const char lbrace = '{';
  static const char rbrace = '}';
  static const char lbracket = '[';
  static const char rbracket = ']';
  static const char plus = '+';
  static const char split = '|';
  static const char dblquote = '\"';
};

generated::Query& append(generated::Query& query,
                         const generated::Query::Unit& unit);

template <typename InputIterator>
generated::Query& append(generated::Query& query, InputIterator first,
                         InputIterator last) {
  while (first < last) {
    append(query, *first);
    ++first;
  }
  return query;
}

size_t count_unit(const generated::Query& query,
                  generated::Query::Unit::Tag tag);

bool has_unit(const generated::Query& query, generated::Query::Unit::Tag tag);

bool has_wildcard(const generated::Query& query);

bool is_normalized(const generated::Query& query);

/**
 * Creates a valid \c Query::Unit from a pair of \c Query::Unit::Tag and \c
 * std::string. For units that represent a single wildcard, like \c ASTERISK or
 * \c QMARK, the text part will be ignored and remains empty. All other
 * attributes will be initialized to default values according to their \c
 * .proto file definition.
 */
generated::Query::Unit make_unit(generated::Query::Unit::Tag tag,
                                 const std::string& text = "");

size_t min_expanded_length(const generated::Query& query);

size_t max_expanded_length(const generated::Query& query);

bool ends_with(const generated::Query& query, generated::Query::Unit::Tag tag);

bool starts_with(const generated::Query& query,
                 generated::Query::Unit::Tag tag);

// -----------------------------------------------------------------------------
// I/O support
// -----------------------------------------------------------------------------

std::ostream& print_as_text_to(const generated::Query::Unit& unit,
                               std::ostream& os);

std::ostream& println_as_text_to(const generated::Query::Unit& unit,
                                 std::ostream& os);

std::ostream& print_as_json_to(const generated::Query::Unit& unit,
                               std::ostream& os);

std::ostream& println_as_json_to(const generated::Query::Unit& unit,
                                 std::ostream& os);

std::ostream& print_as_text_to(const generated::Query& query, std::ostream& os);

std::ostream& println_as_text_to(const generated::Query& query,
                                 std::ostream& os);

std::ostream& print_as_json_to(const generated::Query& query, std::ostream& os);

std::ostream& println_as_json_to(const generated::Query& query,
                                 std::ostream& os);

std::string to_string(const generated::Query& query);

// -----------------------------------------------------------------------------
// Operator support
// -----------------------------------------------------------------------------

namespace generated {
// Operators must be defined in the same namespace as its operands for
// argument-dependent lookup (ADL, also known as Koenig lookup).

bool operator==(const Query& a, const Query& b);

bool operator==(const Query::Unit& a, const Query::Unit& b);

std::ostream& operator<<(std::ostream& os, const Query& query);

std::ostream& operator<<(std::ostream& os, const Query::Unit& unit);

std::ostream& operator<<(std::ostream& os, Query::Unit::Tag tag);

bool operator<(const Query::Unit& a, const Query::Unit& b);

} // namespace generated
} // namespace netspeak

#endif // NETSPEAK_QUERY_METHODS_HPP
