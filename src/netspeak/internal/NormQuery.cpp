
#include "netspeak/internal/NormQuery.hpp"

#include <ostream>

#include "aitools/util/exception.hpp"


namespace netspeak {
namespace internal {


typedef NormQuery::Unit Unit;


const Unit::Text QMARK_STR = std::make_shared<std::string>("?");

Unit Unit::word(const Text& text, const Source& source) {
  return Unit(Tag::WORD, text, source);
}
Unit Unit::qmark(const Source& source) {
  return Unit(Tag::QMARK, QMARK_STR, source);
}


const NormQuery NormQuery::EMPTY = NormQuery();

bool NormQuery::empty() const {
  return units().empty();
}
size_t NormQuery::size() const {
  return units().size();
}

bool NormQuery::has_wildcards() const {
  for (const auto& unit : units()) {
    if (unit.tag == Unit::Tag::QMARK) {
      return true;
    }
  }
  return false;
}


std::ostream& operator<<(std::ostream& out, const NormQuery::Unit::Tag& tag) {
  switch (tag) {
    case Unit::Tag::WORD:
      return out << "Word";
    case Unit::Tag::QMARK:
      return out << "QMark";

    default:
      throw std::logic_error("Unknown tag");
  }
}
std::ostream& operator<<(std::ostream& out, const NormQuery::Unit& unit) {
  switch (unit.tag) {
    case Unit::Tag::WORD:
      return out << "\"" << *unit.text << "\"";
    case Unit::Tag::QMARK:
      return out << "?";

    default:
      throw std::logic_error("Unknown tag");
  }

  return out;
}
std::ostream& operator<<(std::ostream& out, const NormQuery& query) {
  // This will out the norm query "foo ?" as:
  //
  //   NormQuery ( "foo" ? )
  //
  // Each unit will be printed as its text (quoted for WORDs).

  out << "NormQuery(";
  for (const auto& unit : query.units()) {
    out << " " << unit;
  }
  out << " )";

  return out;
}

} // namespace internal
} // namespace netspeak
