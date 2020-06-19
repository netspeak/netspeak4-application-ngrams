#include "netspeak/query_methods.hpp"

#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include "aitools/util/exception.hpp"

#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/query_normalization.hpp"

namespace netspeak {

generated::Query& append(generated::Query& query,
                         const generated::Query::Unit& unit) {
  query.add_unit()->CopyFrom(unit);
  return query;
}

size_t count_unit(const generated::Query& query,
                  generated::Query::Unit::Tag tag) {
  size_t count(0);
  for (const auto& unit : query.unit()) {
    if (unit.tag() == tag)
      ++count;
  }
  return count;
}

bool has_unit(const generated::Query& query, generated::Query::Unit::Tag tag) {
  return count_unit(query, tag) != 0;
}

bool has_wildcard(const generated::Query& query) {
  for (const auto& unit : query.unit()) {
    switch (unit.tag()) {
      case generated::Query::Unit::ASTERISK:
      case generated::Query::Unit::DICTSET:
      case generated::Query::Unit::OPTIONSET:
      case generated::Query::Unit::ORDERSET:
      case generated::Query::Unit::QMARK:
      case generated::Query::Unit::QMARK_FOR_ASTERISK:
      case generated::Query::Unit::PLUS:
      case generated::Query::Unit::QMARK_FOR_PLUS:
        return true;
      default:;
    }
  }
  return false;
}

bool is_normalized(const generated::Query& query) {
  for (const auto& unit : query.unit()) {
    switch (unit.tag()) {
      case generated::Query::Unit::ASTERISK:
      case generated::Query::Unit::PLUS:
      case generated::Query::Unit::DICTSET:
      case generated::Query::Unit::REGEXWORD:
      case generated::Query::Unit::OPTIONSET:
      case generated::Query::Unit::ORDERSET:
      case generated::Query::Unit::ASSOCIATION:
        return false;
      default:;
    }
  }
  return true;
}

generated::Query::Unit make_unit(generated::Query::Unit::Tag tag,
                                 const std::string& text) {
  generated::Query::Unit unit;
  unit.set_tag(tag);
  switch (tag) {
    case generated::Query::Unit::ASTERISK:
      unit.set_text(std::string(1, wildcard::asterisk));
      break;
    case generated::Query::Unit::PLUS:
      unit.set_text(std::string(1, wildcard::plus));
      break;
    case generated::Query::Unit::QMARK:
    case generated::Query::Unit::QMARK_FOR_ASTERISK:
    case generated::Query::Unit::QMARK_FOR_PLUS:
      unit.set_text(std::string(1, wildcard::qmark));
      break;
    case generated::Query::Unit::DICTSET:
    case generated::Query::Unit::REGEXWORD:
    case generated::Query::Unit::OPTIONSET:
    case generated::Query::Unit::ORDERSET:
    case generated::Query::Unit::WORD:
    case generated::Query::Unit::WORD_IN_DICTSET:
    case generated::Query::Unit::WORD_IN_OPTIONSET:
    case generated::Query::Unit::WORD_IN_ORDERSET:
    case generated::Query::Unit::WORD_FOR_REGEX:
    case generated::Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET:
    case generated::Query::Unit::WORD_FOR_REGEX_IN_ORDERSET:
      unit.set_text(text);
      break;
    default:
      aitools::throw_invalid_argument("make_unit : unknown tag", tag);
  }
  return unit;
}


bool ends_with(const generated::Query& query, generated::Query::Unit::Tag tag) {
  return query.unit_size() == 0 ? false
                                : (std::prev(query.unit().end()))->tag() == tag;
}

bool starts_with(const generated::Query& query,
                 generated::Query::Unit::Tag tag) {
  return query.unit_size() == 0 ? false : query.unit().begin()->tag() == tag;
}

// -----------------------------------------------------------------------------
// I/O support
// -----------------------------------------------------------------------------

std::ostream& print_as_text_to(const generated::Query::Unit& unit,
                               std::ostream& os) {
  switch (unit.tag()) {
    case generated::Query::Unit::ASTERISK:
      os << wildcard::asterisk;
      break;
    case generated::Query::Unit::PLUS:
      os << wildcard::plus;
      break;
    case generated::Query::Unit::QMARK:
    case generated::Query::Unit::QMARK_FOR_ASTERISK:
    case generated::Query::Unit::QMARK_FOR_PLUS:
      os << wildcard::qmark;
      break;
    case generated::Query::Unit::WORD:
    case generated::Query::Unit::REGEXWORD:
    case generated::Query::Unit::WORD_IN_DICTSET:
    case generated::Query::Unit::WORD_IN_OPTIONSET:
    case generated::Query::Unit::WORD_IN_ORDERSET:
    case generated::Query::Unit::WORD_FOR_REGEX:
    case generated::Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET:
    case generated::Query::Unit::WORD_FOR_REGEX_IN_ORDERSET:
      os << unit.text();
      break;
    case generated::Query::Unit::DICTSET:
      os << wildcard::hash << unit.text();
      break;
    case generated::Query::Unit::OPTIONSET:
      os << wildcard::lbracket << ' ' << unit.text() << ' '
         << wildcard::rbracket;
      break;
    case generated::Query::Unit::ORDERSET:
      os << wildcard::lbrace << ' ' << unit.text() << ' ' << wildcard::rbrace;
      break;
    default:
      aitools::throw_invalid_argument("print_as_text_to : unknown tag",
                                      unit.tag());
  }
  return os;
}

std::ostream& print_as_text_to(const generated::Query& query,
                               std::ostream& os) {
  for (auto it = query.unit().begin(); it != query.unit().end(); ++it) {
    if (it != query.unit().begin()) {
      os << ' ';
    }
    print_as_text_to(*it, os);
  }
  return os;
}

std::string to_string(const generated::Query& query) {
  std::ostringstream oss;
  print_as_text_to(query, oss);
  return oss.str();
}

// -----------------------------------------------------------------------------
// Operator support
// -----------------------------------------------------------------------------

namespace generated {

bool operator==(const Query& a, const Query& b) {
  return a.unit_size() == b.unit_size() &&
         std::equal(a.unit().begin(), a.unit().end(), b.unit().begin());
}

bool operator==(const Query::Unit& a, const Query::Unit& b) {
  return a.frequency() == b.frequency() && a.position() == b.position() &&
         a.pruning() == b.pruning() && a.quantile() == b.quantile() &&
         a.text() == b.text() && a.tag() == b.tag();
}

std::ostream& operator<<(std::ostream& os, const Query& query) {
  return print_as_text_to(query, os);
}

std::ostream& operator<<(std::ostream& os, const Query::Unit& unit) {
  return print_as_text_to(unit, os);
}

std::ostream& operator<<(std::ostream& os, Query::Unit::Tag tag) {
  return os << Query::Unit::Tag_Name(tag);
}

bool operator<(const Query::Unit& a, const Query::Unit& b) {
  return a.frequency() < b.frequency();
}

} // namespace generated
} // namespace netspeak
