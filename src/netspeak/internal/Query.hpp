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
  typedef enum {
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
  std::vector<const std::shared_ptr<const Unit>> units_;
  std::weak_ptr<const Unit> parent_;
  // This is a weak ref to this object. This is necessary to set the parent of
  // children. Not nice, but hey. It works.
  std::weak_ptr<Unit> self_;

  QueryUnit__(Tag tag, const std::string& text);
  QueryUnit__() = delete;
  QueryUnit__(const QueryUnit__&) = delete;
  ~QueryUnit__();

public:
  std::shared_ptr<Unit> static terminal(Unit::Tag tag, const std::string& text);
  std::shared_ptr<Unit> static non_terminal(Unit::Tag tag);


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

  const std::vector<const std::shared_ptr<const Unit>>& units() const {
    return units_;
  }

  std::shared_ptr<const Unit> parent() const {
    return parent_.lock();
  }

  bool is_terminal() const;

  void add_unit(const std::shared_ptr<Unit>& unit);
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
};


std::ostream& operator<<(std::ostream& out, const Query::Unit::Tag& tag);
std::ostream& operator<<(std::ostream& out, const Query::Unit& unit);
std::ostream& operator<<(std::ostream& out, const Query& query);


} // namespace internal
} // namespace netspeak


#endif
