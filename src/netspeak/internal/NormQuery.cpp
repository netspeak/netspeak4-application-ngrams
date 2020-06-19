
#include "netspeak/internal/NormQuery.hpp"

#include <ostream>

#include "aitools/util/exception.hpp"


namespace netspeak {
namespace internal {


const NormQueryUnit__::Text NormQueryUnit__::QMARK_STR =
    std::make_shared<std::string>("?");


const NormQuery NormQuery::EMPTY = NormQuery();

size_t NormQuery::size() const {
  return units().size();
}

bool NormQuery::has_wildcards() const {
  for (const auto& unit : units()) {
    if (unit.tag == NormQuery::Unit::Tag::QMARK) {
      return true;
    }
  }
  return false;
}


std::ostream& operator<<(std::ostream& out, const NormQuery& query) {
  // This will out the norm query "foo ?" as:
  //
  //   NormQuery { "foo"(0) ?(1) }
  //
  // Each unit will be printed as its text (quoted for WORDs) following by its
  // source id.

  out << "NormQuery {";
  for (const auto& unit : query.units()) {
    out << " ";

    switch (unit.tag) {
      case NormQuery::Unit::Tag::WORD:
        out << "\"" << *unit.text << "\"";
        break;

      case NormQuery::Unit::Tag::QMARK:
        out << "?";
        break;

      default:
        aitools::throw_invalid_argument("Unknown tag", unit.tag);
        break;
    }

    // just print the address of the source unit
    out << "(" << std::hex << (void*)unit.source.get() << ")";
  }
  out << " }";

  return out;
}

} // namespace internal
} // namespace netspeak
