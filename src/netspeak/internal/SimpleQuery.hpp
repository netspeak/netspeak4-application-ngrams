#ifndef NETSPEAK_INTERNAL_SIMPLE_QUERY_HPP
#define NETSPEAK_INTERNAL_SIMPLE_QUERY_HPP

#include <memory>
#include <string>
#include <vector>

#include "netspeak/internal/NormQuery.hpp"
#include "netspeak/internal/Query.hpp"

namespace netspeak {
namespace internal {

class SimpleQueryUnit__ {
public:
  typedef enum class Tag {
    WORD,
    QMARK,
    STAR,
    REGEX,

    CONCAT,
    ALTERNATION,
  } Tag;
  typedef std::shared_ptr<const Query::Unit> Source;
  typedef std::string Text;

private:
  typedef SimpleQueryUnit__ Unit;

  Tag tag_;
  Source source_;
  Text text_;
  std::vector<std::shared_ptr<Unit>> children_;
  std::weak_ptr<Unit> self_;
  std::weak_ptr<Unit> parent_;

  SimpleQueryUnit__() = delete;
  SimpleQueryUnit__(const SimpleQueryUnit__&) = delete;
  SimpleQueryUnit__(Tag tag, const Text& text, const Source& source)
      : tag_(tag), text_(text), source_(source){};

  static std::shared_ptr<Unit> new_unit(Tag tag, const Text& text,
                                        const Source& source);

public:
  static std::shared_ptr<Unit> new_word(const Text& text, const Source& source);
  static std::shared_ptr<Unit> new_regex(const Text& text,
                                         const Source& source);
  static std::shared_ptr<Unit> new_qmark(const Source& source);
  static std::shared_ptr<Unit> new_star(const Source& source);
  static std::shared_ptr<Unit> new_concat(const Source& source);
  static std::shared_ptr<Unit> new_alternation(const Source& source);

  /**
   * @brief Creates a deep copy of this unit.
   *
   * @return std::shared_ptr<Unit>
   */
  std::shared_ptr<Unit> clone() const;

  Tag tag() const {
    return tag_;
  }
  const Text& text() const {
    return text_;
  }
  const Source& source() const {
    return source_;
  }
  const std::vector<std::shared_ptr<Unit>>& children() const {
    return children_;
  }
  std::shared_ptr<Unit> parent() {
    return parent_.lock();
  }
  std::shared_ptr<const Unit> parent() const {
    return parent_.lock();
  }

  bool is_terminal() const;

  /**
   * @brief Returns the maximum length of a phrase that can (theoretically) be
   * matched (= accepted) by this unit.
   *
   * If the returned maximum is 0, then this unit either doesn't accept any
   * phrases at all or only the empty phrase.
   *
   * If the returned maximum is \c UINT32_MAX, then the maximum phrase length is
   * unbounded.
   *
   * @return uint32_t
   */
  uint32_t max_length() const;
  /**
   * @brief Returns the minimum length of a phrase that can (theoretically) be
   * matched (= accepted) by this unit.
   *
   * If the returned maximum is \c UINT32_MAX, then this unit doesn't accept any
   * phrases.
   *
   * @return uint32_t
   */
  uint32_t min_length() const;

  void add_child(const std::shared_ptr<Unit>& unit);
  void clear_children();
  std::vector<std::shared_ptr<Unit>> drain_children();
  void replace_with(const std::shared_ptr<Unit>& unit);
  void remove();
};

bool operator==(const SimpleQueryUnit__& lhs, const SimpleQueryUnit__& rhs);
inline bool operator!=(const SimpleQueryUnit__& lhs,
                       const SimpleQueryUnit__& rhs) {
  return !operator==(lhs, rhs);
}


class SimpleQuery {
public:
  typedef SimpleQueryUnit__ Unit;

private:
  std::shared_ptr<Unit> root_;

  SimpleQuery() = delete;
  SimpleQuery(const SimpleQuery&) = delete;

public:
  SimpleQuery(const std::shared_ptr<Unit>& root): root_(root) {}

  std::shared_ptr<Unit>& root() {
    return root_;
  }
  const std::shared_ptr<const Unit>& root() const {
    return root_;
  }

  bool has_wildcards() const;
};


std::ostream& operator<<(std::ostream& out, const SimpleQuery::Unit::Tag& tag);
std::ostream& operator<<(std::ostream& out, const SimpleQuery::Unit& unit);
std::ostream& operator<<(std::ostream& out, const SimpleQuery& query);


} // namespace internal
} // namespace netspeak


#endif
