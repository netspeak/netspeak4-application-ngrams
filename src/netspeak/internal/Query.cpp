#include "netspeak/internal/Query.hpp"

#include <ostream>


namespace netspeak {
namespace internal {

typedef QueryUnit__ Unit;


Unit::QueryUnit__(Tag tag, const std::string& text) : tag_(tag), text_(text) {}

std::shared_ptr<Unit> Unit::terminal(Tag tag, const std::string& text) {
  const auto unit = std::make_shared<Unit>(Unit(tag, text));
  unit->self_ = unit;
  return unit;
}
std::shared_ptr<Unit> Unit::non_terminal(Tag tag) {
  const auto unit = std::make_shared<Unit>(Unit(tag, ""));
  unit->self_ = unit;
  return unit;
}

void Unit::add_unit(const std::shared_ptr<Unit>& unit) {
  if (is_terminal()) {
    throw new std::logic_error("A non terminal cannot have children.");
  }
  if (unit->parent()) {
    throw new std::logic_error("The unit is already has a parent.");
  }

  unit->parent_ = self_;
  units_.push_back(unit);
}

bool Unit::is_terminal() const {
  switch (tag()) {
    case WORD:
    case QMARK:
    case STAR:
    case PLUS:
    case REGEX:
    case DICTSET:
      return true;

    default:
      return false;
  }
}


std::ostream& operator<<(std::ostream& out, const Query::Unit::Tag& tag) {
  switch (tag) {
    case Query::Unit::Tag::WORD:
      return out << "Word";
    case Query::Unit::Tag::QMARK:
      return out << "QMark";
    case Query::Unit::Tag::STAR:
      return out << "Star";
    case Query::Unit::Tag::PLUS:
      return out << "Plus";
    case Query::Unit::Tag::REGEX:
      return out << "Regex";
    case Query::Unit::Tag::DICTSET:
      return out << "DictSet";

    case Query::Unit::Tag::ORDERSET:
      return out << "OrderSet";
    case Query::Unit::Tag::OPTIONSET:
      return out << "OptionSet";
    case Query::Unit::Tag::ALTERNATION:
      return out << "Alternation";
    case Query::Unit::Tag::CONCAT:
      return out << "Concat";

    default:
      throw std::logic_error("Unknown tag");
  }
}
std::ostream& operator<<(std::ostream& out, const Query::Unit& unit) {
  out << unit.tag();

  switch (unit.tag()) {
    case Query::Unit::Tag::QMARK:
    case Query::Unit::Tag::STAR:
    case Query::Unit::Tag::PLUS:
      // since, the text is trivial, we don't print anything
      break;

    case Query::Unit::Tag::WORD:
    case Query::Unit::Tag::REGEX:
    case Query::Unit::Tag::DICTSET:
      out << "(\"" << unit.text() << "\")";
      break;

    case Query::Unit::Tag::ORDERSET:
    case Query::Unit::Tag::OPTIONSET:
    case Query::Unit::Tag::ALTERNATION:
    case Query::Unit::Tag::CONCAT:
      out << " {";
      auto it = unit.units().begin();
      auto end = unit.units().end();
      if (it != end) {
        out << *it;
        it++;
        for (; it != end; it++) {
          out << ", " << *it;
        }
      }
      out << " }";
      break;

    default:
      throw std::logic_error("Unknown tag");
  }

  return out;
}
std::ostream& operator<<(std::ostream& out, const Query& query) {
  out << "Query(" << query.alternatives() << ")";
  return out;
}


} // namespace internal
} // namespace netspeak
