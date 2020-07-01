#ifndef NETSPEAK_INTERNAL_LENGTH_RANGE_HPP
#define NETSPEAK_INTERNAL_LENGTH_RANGE_HPP

#include <ostream>

namespace netspeak {
namespace internal {


/**
 * A length range is an interval of lengths where both ends (min and max) are
 * inclusive.
 */
struct LengthRange {
public:
  /**
   * @brief Returns the minimum length of the range.
   *
   * If the minimum is \c UINT32_MAX, then this range is empty.
   */
  uint32_t min;
  /**
   * @brief Returns the maximum length of a phrase that can (theoretically) be
   * matched (= accepted) by this unit.
   *
   * If the maximum is 0, then this unit either doesn't accept any
   * phrases at all or only the empty phrase.
   *
   * If the maximum is \c UINT32_MAX, then the range is
   * unbounded.
   */
  uint32_t max;

  LengthRange() : min(UINT32_MAX), max(0) {}
  LengthRange(uint32_t min) : min(min), max(UINT32_MAX) {}
  LengthRange(uint32_t min, uint32_t max) : min(min), max(max) {}

  /**
   * @brief Returns whether this range is empty.
   *
   * An empty is range is the empty set. This means no length is part of the
   * range. In most cases, this means that no words/phrases of any length are
   * accepted. If the range describes a range of distances (e.g. to an element),
   * then the element cannot be reached.
   */
  bool empty() const {
    return min == UINT32_MAX;
  }

  /**
   * @brief Returns whether this range is unbounded.
   *
   * If a range is unbounded, it will accept all lengths >= its minimum.
   */
  bool unbounded() const {
    return min != UINT32_MAX && max == UINT32_MAX;
  }

  bool accepts(uint32_t length) const {
    return !empty() && min <= length && length <= max;
  }


};

LengthRange& operator&=( LengthRange& lhs, const LengthRange& rhs) {
  // concatenation
  // This assumes that no overflows will occur
  if (lhs.empty()) {
    return lhs;
  }
  if (rhs.empty()) {
    lhs.min = UINT32_MAX;
    return lhs;
  }
  if (lhs.max == UINT32_MAX) {
    lhs.min += rhs.min;
    return lhs;
  }
  if (rhs.max == UINT32_MAX) {
    lhs.min += rhs.min;
    lhs.max = UINT32_MAX;
    return lhs;
  }
  lhs.min += rhs.min;
  lhs.max += rhs.max;
  return lhs;
}
LengthRange& operator|=(LengthRange& lhs, const LengthRange& rhs) {
  // union
  if (rhs.empty()) {
    return lhs;
  }
  if (lhs.empty()) {
    lhs = rhs;
    return lhs;
  }
  if (rhs.min < lhs.min) {
    lhs.min = rhs.min;
  }
  if (rhs.max > lhs.max) {
    lhs.max = rhs.max;
  }
  return lhs;
}

LengthRange operator&(const LengthRange& lhs, const LengthRange& rhs) {
  auto copy = lhs;
  copy |= rhs;
  return copy;
}
LengthRange operator|(const LengthRange& lhs, const LengthRange& rhs) {
  auto copy = lhs;
  copy |= rhs;
  return copy;
}

std::ostream& operator<<(std::ostream& out, const LengthRange& range) {
  if (range.empty()) {
    return out << "LengthRange(empty)";
  } else if (range.unbounded()) {
    return out << "LengthRange(" << range.min << ", unbounded)";
  } else {
    return out << "LengthRange(" << range.min << ", " << range.max << ")";
  }
}


} // namespace internal
} // namespace netspeak


#endif
