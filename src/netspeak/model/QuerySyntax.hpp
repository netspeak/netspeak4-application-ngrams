#ifndef NETSPEAK_INTERNAL_QUERY_SYNTAX_HPP
#define NETSPEAK_INTERNAL_QUERY_SYNTAX_HPP

#include <string>
#include <ostream>

#include "aitools/value/pair.hpp"

namespace netspeak {
namespace model {

struct QuerySyntax {
  static const char HASH = '#';

  static const char QMARK = '?';
  static const char STAR = '*';
  static const char PLUS = '+';

  static const char ALTERNATION = '|';

  static const char DBL_QUOTE = '"';

  static const char BRACE_LEFT = '{';
  static const char BRACE_RIGHT = '}';
  static const char BRACKET_LEFT = '[';
  static const char BRACKET_RIGHT = ']';

};


} // namespace model
} // namespace netspeak

#endif
