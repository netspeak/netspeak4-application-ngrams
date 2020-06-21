#include "netspeak/internal/Query.hpp"

#include <cassert>
#include <ostream>


namespace netspeak {
namespace internal {

typedef Query::Unit Unit;


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

void Unit::add_child(const std::shared_ptr<Unit>& unit) {
  assert(unit);

  if (is_terminal()) {
    throw new std::logic_error("A non-terminal cannot have children.");
  }
  if (unit->parent()) {
    throw new std::logic_error("The unit is already has a parent.");
  }

  unit->parent_ = self_;
  children_.push_back(unit);
}

bool Unit::is_terminal() const {
  switch (tag()) {
    case Tag::WORD:
    case Tag::QMARK:
    case Tag::STAR:
    case Tag::PLUS:
    case Tag::REGEX:
    case Tag::DICTSET:
      return true;

    default:
      return false;
  }
}

uint32_t Unit::max_length() const {
  switch (tag()) {
    case Tag::WORD:
      return 1;
    case Tag::QMARK:
      return 1;
    case Tag::STAR:
      return UINT32_MAX;
    case Tag::PLUS:
      return UINT32_MAX;
    case Tag::REGEX:
      return 1;
    case Tag::DICTSET:
      // A dict set may be replaced with a phrases that contain more than one
      // word. The number of words one of the replacement phrases may contain
      // isn't limited by anything.
      return UINT32_MAX;

    case Tag::OPTIONSET: // for option sets, the same rules as for alts apply
    case Tag::ALTERNATION: {
      uint32_t max = 0;
      for (const auto& child : children()) {
        max = std::max(max, child->max_length());
        if (max == UINT32_MAX) {
          return UINT32_MAX;
        }
      }
      return max;
    }

    case Tag::ORDERSET: // for order sets, the same rules as for concats apply
    case Tag::CONCAT: {
      uint32_t total = 0;
      for (const auto& child : children()) {
        uint32_t value = child->max_length();
        if (value == UINT32_MAX) {
          return UINT32_MAX;
        }
        total += value;
      }
      return total;
    }

    default:
      throw std::logic_error("Unknown tag");
  }
}
uint32_t Unit::min_length() const {
  switch (tag()) {
    case Tag::WORD:
      return 1;
    case Tag::QMARK:
      return 1;
    case Tag::STAR:
      return 0;
    case Tag::PLUS:
      return 1;
    case Tag::REGEX:
      return 1;
    case Tag::DICTSET:
      // A dict set is guaranteed to match itself and all replacement phrases
      // may not be empty.
      return 1;

    case Tag::OPTIONSET:
      if (children().size() == 1) {
        return 0;
      }
      // apart from the only-one-element-case, option sets are the same as alts
      // fall through
    case Tag::ALTERNATION: {
      uint32_t min = UINT32_MAX;
      for (const auto& child : children()) {
        min = std::min(min, child->min_length());
        if (min == 0) {
          return 0;
        }
      }
      return min;
    }

    case Tag::ORDERSET: // for order sets, the same rules as for concats apply
    case Tag::CONCAT: {
      uint32_t total = 0;
      for (const auto& child : children()) {
        uint32_t value = child->min_length();
        if (value == UINT32_MAX) {
          return UINT32_MAX;
        }
        total += value;
      }
      return total;
    }

    default:
      throw std::logic_error("Unknown tag");
  }
}


std::ostream& operator<<(std::ostream& out, const Unit::Tag& tag) {
  switch (tag) {
    case Unit::Tag::WORD:
      return out << "Word";
    case Unit::Tag::QMARK:
      return out << "QMark";
    case Unit::Tag::STAR:
      return out << "Star";
    case Unit::Tag::PLUS:
      return out << "Plus";
    case Unit::Tag::REGEX:
      return out << "Regex";
    case Unit::Tag::DICTSET:
      return out << "DictSet";

    case Unit::Tag::ORDERSET:
      return out << "OrderSet";
    case Unit::Tag::OPTIONSET:
      return out << "OptionSet";
    case Unit::Tag::ALTERNATION:
      return out << "Alternation";
    case Unit::Tag::CONCAT:
      return out << "Concat";

    default:
      throw std::logic_error("Unknown tag");
  }
}
std::ostream& operator<<(std::ostream& out, const Unit& unit) {
  out << unit.tag();

  switch (unit.tag()) {
    case Unit::Tag::QMARK:
    case Unit::Tag::STAR:
    case Unit::Tag::PLUS:
      // since, the text is trivial, we don't print anything
      break;

    case Unit::Tag::WORD:
    case Unit::Tag::REGEX:
    case Unit::Tag::DICTSET:
      out << "(\"" << unit.text() << "\")";
      break;

    case Unit::Tag::ORDERSET:
    case Unit::Tag::OPTIONSET:
    case Unit::Tag::ALTERNATION:
    case Unit::Tag::CONCAT:
      out << "{";
      auto it = unit.children().begin();
      auto end = unit.children().end();
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
