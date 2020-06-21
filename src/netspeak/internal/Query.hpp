#ifndef NETSPEAK_INTERNAL_QUERY_HPP
#define NETSPEAK_INTERNAL_QUERY_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "boost/optional.hpp"


namespace netspeak {
namespace internal {

class QueryUnit__ {
public:
  typedef enum class Tag {
    // Terminals
    WORD,
    QMARK,
    STAR,
    PLUS,
    REGEX,
    DICTSET,

    // Non-terminals
    ORDERSET,
    OPTIONSET,
    ALTERNATION,
    CONCAT,
  } Tag;

private:
  typedef QueryUnit__ Unit;

  Tag tag_;
  std::string text_;
  std::vector<const std::shared_ptr<const Unit>> children_;
  std::weak_ptr<const Unit> parent_;
  // This is a weak ref to this object. This is necessary to set the parent of
  // children. Not nice, but hey. It works.
  std::weak_ptr<Unit> self_;

  QueryUnit__(Tag tag, const std::string& text);
  QueryUnit__() = delete;
  QueryUnit__(const QueryUnit__&) = delete;
  ~QueryUnit__();

public:
  static std::shared_ptr<Unit> terminal(Unit::Tag tag, const std::string& text);
  static std::shared_ptr<Unit> non_terminal(Unit::Tag tag);


  Tag tag() const {
    return tag_;
  }

  /**
   * The unescaped text value of the unit.
   *
   * "Unescaped" means that backslashes used to escape characters (e.g. in words
   * and regexes) and prefixes used to identify the token (e.g. the `#` of dict
   * sets) are already handled.
   *
   * This value is only relevant for terminals. The text of non-terminals is
   * empty.
   *
   * @return const std::string&
   */
  const std::string& text() const {
    return text_;
  }

  const std::vector<const std::shared_ptr<const Unit>>& children() const {
    return children_;
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
};


/**
 * The internal representation of a parsed Netspeak query.
 *
 * This is a literal representation. While completely parsed and interpreted, it
 * will literally represent all syntactical structures of the textual query.
 */
class Query {
public:
  typedef QueryUnit__ Unit;

private:
  const std::shared_ptr<Unit> alternatives_;

  Query(const Query&) = delete;

public:
  Query()
      : alternatives_(std::make_shared<Unit>(
            Unit::non_terminal(Unit::Tag::ALTERNATION))){};

  const std::shared_ptr<Unit>& alternatives() {
    return alternatives_;
  }
  const std::shared_ptr<const Unit>& alternatives() const {
    return alternatives_;
  }


  uint32_t max_length() const {
    return alternatives()->max_length();
  }
  uint32_t min_length() const {
    return alternatives()->min_length();
  }
};


std::ostream& operator<<(std::ostream& out, const Query::Unit::Tag& tag);
std::ostream& operator<<(std::ostream& out, const Query::Unit& unit);
std::ostream& operator<<(std::ostream& out, const Query& query);


} // namespace internal
} // namespace netspeak


#endif
