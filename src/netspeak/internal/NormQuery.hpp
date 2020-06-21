#ifndef NETSPEAK_INTERNAL_NORM_QUERY_HPP
#define NETSPEAK_INTERNAL_NORM_QUERY_HPP

#include <memory>
#include <string>
#include <vector>

#include "netspeak/internal/Query.hpp"

namespace netspeak {
namespace internal {

// Implementation note:
//
// For every query, Netspeak will create potentially thousands for norm query.
// To keep things performant, norm queries and their units are extremely
// light-weight. Units are little more than two shared pointers and a tag and
// norm queries are just a wrapper around a vector.


/**
 * A unit of a norm query.
 *
 * Note: A unit doesn't own the data it points to!
 */
struct NormQueryUnit__ {
public:
  typedef enum class Tag {
    WORD,
    QMARK,
  } Tag;
  typedef std::shared_ptr<const std::string> Text;
  typedef std::shared_ptr<const Query::Unit> Source;

public:
  Tag tag;
  Text text;
  Source source;

  static NormQueryUnit__ word(const Text& text, const Source& source);
  static NormQueryUnit__ qmark(const Source& source);

private:
  NormQueryUnit__(Tag tag, const Text& text, const Source& source)
      : tag(tag), text(text), source(source) {}
  NormQueryUnit__() = delete;
};


/**
 * A norm query (= normalized query) is a Netspeak query that only contains
 * one concatenation. This concatenation is only allowed to contain question
 * mark wildcards and constant words.
 *
 * This represents the type of query a Netspeak index can answer without any
 * further query-processing and can be thought of as the "native language" of a
 * Netspeak Index.
 *
 * Since a Netspeak query typically generates many norm queries, each unit of a
 * norm query has an id of the token in the original Netspeak query that
 * generated it. This can be used to track which word is matched by which token
 * of the original Netspeak query.
 */
class NormQuery {
public:
  typedef NormQueryUnit__ Unit;

private:
  std::vector<Unit> units_;

public:
  static const NormQuery EMPTY;

  std::vector<Unit>& units() {
    return units_;
  }
  const std::vector<Unit>& units() const {
    return units_;
  }

  bool empty() const;
  size_t size() const;
  bool has_wildcards() const;
};


std::ostream& operator<<(std::ostream& out, const NormQuery::Unit::Tag& tag);
std::ostream& operator<<(std::ostream& out, const NormQuery::Unit& unit);
std::ostream& operator<<(std::ostream& out, const NormQuery& query);


} // namespace internal
} // namespace netspeak


#endif
