#include "netspeak/internal/SimpleQuery.hpp"

#include <algorithm>
#include <cassert>
#include <ostream>


namespace netspeak {
namespace internal {

typedef SimpleQuery::Unit Unit;


inline std::shared_ptr<Unit> Unit::new_unit(Tag tag, const Text& text,
                                            const Source& source) {
  const auto unit = std::make_shared<Unit>(Unit(tag, text, source));
  unit->self_ = unit;
  return unit;
}
std::shared_ptr<Unit> Unit::new_word(const Text& text, const Source& source) {
  return Unit::new_unit(Tag::WORD, text, source);
}
std::shared_ptr<Unit> Unit::new_regex(const Text& text, const Source& source) {
  return Unit::new_unit(Tag::REGEX, text, source);
}
std::shared_ptr<Unit> Unit::new_qmark(const Source& source) {
  return Unit::new_unit(Tag::QMARK, "?", source);
}
std::shared_ptr<Unit> Unit::new_star(const Source& source) {
  return Unit::new_unit(Tag::STAR, "*", source);
}
std::shared_ptr<Unit> Unit::new_concat(const Source& source) {
  return Unit::new_unit(Tag::CONCAT, "", source);
}
std::shared_ptr<Unit> Unit::new_alternation(const Source& source) {
  return Unit::new_unit(Tag::ALTERNATION, "", source);
}

std::shared_ptr<Unit> Unit::clone() const {
  const auto unit = Unit::new_unit(tag(), text(), source());
  for (const auto& child : children()) {
    unit->add_child(child->clone());
  }
  return unit;
}


bool Unit::is_terminal() const {
  switch (tag_) {
    case Unit::Tag::WORD:
    case Unit::Tag::QMARK:
    case Unit::Tag::STAR:
    case Unit::Tag::REGEX:
      return true;

    default:
      return false;
  }
}


LengthRange Unit::length_range() const {
  switch (tag_) {
    case Unit::Tag::STAR:
      return LengthRange(0);

    case Unit::Tag::WORD:
    case Unit::Tag::QMARK:
    case Unit::Tag::REGEX:
      return LengthRange(1, 1);

    case Unit::Tag::ALTERNATION: {
      LengthRange range;
      for (const auto& child : children()) {
        range |= child->length_range();
      }
      return range;
    }

    case Unit::Tag::CONCAT: {
      LengthRange range(0, 0);
      for (const auto& child : children()) {
        range &= child->length_range();
        if (range.empty()) {
          return range;
        }
      }
      return range;
    }

    default:
      throw std::logic_error("Unknown tag");
  }
}

uint32_t Unit::min_length() const {
  switch (tag_) {
    case Unit::Tag::STAR:
      return 0;

    case Unit::Tag::WORD:
    case Unit::Tag::QMARK:
    case Unit::Tag::REGEX:
      return 1;

    case Unit::Tag::ALTERNATION: {
      uint32_t min = UINT32_MAX;
      for (const auto& child : children()) {
        const auto c_min = child->min_length();
        if (c_min < min) {
          if (c_min == 0) {
            return 0;
          }
          min = c_min;
        }
      }
      return min;
    }

    case Unit::Tag::CONCAT: {
      uint32_t min = 0;
      for (const auto& child : children()) {
        const auto c_min = child->min_length();
        if (c_min == UINT32_MAX) {
          return UINT32_MAX;
        }
        min += c_min;
      }
      return min;
    }

    default:
      throw std::logic_error("Unknown tag");
  }
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
void Unit::clear_children() {
  for (auto& child : children_) {
    child->parent_.reset();
  }
  children_.clear();
}
std::vector<std::shared_ptr<Unit>> Unit::drain_children() {
  std::vector<std::shared_ptr<Unit>> ret;
  for (auto& child : children_) {
    child->parent_.reset();
    ret.push_back(child);
  }
  children_.clear();
  return ret;
}
void Unit::replace_with(const std::shared_ptr<Unit>& unit) {
  const auto p = parent();
  if (!p) {
    throw std::logic_error(
        "Cannot replace or remove if this unit doesn't have a parent.");
  }

  // replace or remove, we will detach this unit from its parent
  parent_.reset();

  // find this unit
  auto& p_children = p->children_;
  auto pos = std::find(p_children.begin(), p_children.end(), self_.lock());
  if (pos == p_children.end()) {
    throw std::logic_error("Cannot find this unit in parent.");
  }

  if (unit) {
    // replace
    if (unit->parent()) {
      throw std::logic_error(
          "The given unit is already child of another parent.");
    }

    *pos = unit;
  } else {
    // remove
    p_children.erase(pos);
  }
}
void Unit::remove() {
  replace_with(std::shared_ptr<Unit>());
}


bool operator==(const Unit& lhs, const Unit& rhs) {
  if (lhs.tag() != rhs.tag()) {
    return false;
  }
  if (lhs.is_terminal()) {
    return lhs.text() == rhs.text();
  } else {
    const auto& l_children = lhs.children();
    const auto& r_children = rhs.children();
    if (l_children.size() != r_children.size()) {
      return false;
    }
    for (size_t i = 0; i < l_children.size(); i++) {
      const auto& l_child = l_children[i];
      const auto& r_child = r_children[i];
      if (*l_child != *r_child) {
        return false;
      }
    }
    return true;
  }
}


bool unit_contains_wildcards(const Unit& unit) {
  if (unit.is_terminal()) {
    for (const auto& child : unit.children()) {
      if (unit_contains_wildcards(*child)) {
        return true;
      }
    }
    return false;
  } else {
    return unit.tag() == Unit::Tag::QMARK || unit.tag() == Unit::Tag::STAR;
  }
}
bool SimpleQuery::has_wildcards() const {
  return root() && unit_contains_wildcards(*root());
}


std::ostream& operator<<(std::ostream& out, const SimpleQuery::Unit::Tag& tag) {
  switch (tag) {
    case Unit::Tag::WORD:
      return out << "Word";
    case Unit::Tag::QMARK:
      return out << "QMark";
    case Unit::Tag::STAR:
      return out << "Star";
    case Unit::Tag::REGEX:
      return out << "Regex";

    case Unit::Tag::ALTERNATION:
      return out << "Alternation";
    case Unit::Tag::CONCAT:
      return out << "Concat";

    default:
      throw std::logic_error("Unknown tag");
  }
}
std::ostream& operator<<(std::ostream& out, const SimpleQuery::Unit& unit) {
  switch (unit.tag()) {
    case Unit::Tag::WORD:
      // TODO: Universal word escaping. This also affects Query and NormQuery
      return out << "\"" << unit.text() << "\"";
    case Unit::Tag::QMARK:
      return out << "?";
    case Unit::Tag::STAR:
      return out << "*";
    case Unit::Tag::REGEX:
      return out << "Regex(\"" << unit.text() << "\")";

    case Unit::Tag::ALTERNATION: {
      auto it = unit.children().begin();
      auto end = unit.children().end();

      if (it == end) {
        out << "Alt{}";
      } else {
        out << "Alt{ " << *it;
        it++;
        for (; it != end; it++) {
          out << " | " << *it;
        }
        out << " }";
      }

      return out;
    }

    case Unit::Tag::CONCAT: {
      auto it = unit.children().begin();
      auto end = unit.children().end();

      if (it == end) {
        out << "Concat{}";
      } else {
        out << "Concat{";
        for (; it != end; it++) {
          out << " " << *it;
        }
        out << " }";
      }

      return out;
    }

    default:
      throw std::logic_error("Unknown tag");
  }
}
std::ostream& operator<<(std::ostream& out, const SimpleQuery& query) {
  return out << "SimpleQuery( " << query.root() << " )";
}


} // namespace internal
} // namespace netspeak
