#include "netspeak/QueryNormalizer.hpp"

#include <algorithm>
#include <functional>
#include <limits>
#include <unordered_map>

#include "boost/algorithm/string.hpp"

#include "netspeak/error.hpp"
#include "netspeak/internal/SimpleQuery.hpp"
#include "netspeak/regex/parsers.hpp"
#include "netspeak/util/ChainCutter.hpp"
#include "netspeak/util/Math.hpp"
#include "netspeak/util/Vec.hpp"


namespace netspeak {

/**
 * @brief A converter from a \c Query to its \c SimpleQuery units.
 */
class QuerySimplifier {
public:
  QueryNormalizer::Options options;
  std::shared_ptr<const Dictionaries::Map> dictionary;
  bool allow_regex;

  QuerySimplifier() = delete;
  QuerySimplifier(const QueryNormalizer::Options& options)
      : options(options), allow_regex(false) {}

  typedef std::shared_ptr<SimpleQuery::Unit> SimpleUnitPtr;
  typedef std::shared_ptr<const Query::Unit> UnitPtr;

private:
  static void add_to_concat(SimpleQuery::Unit& concat, SimpleUnitPtr unit) {
    // If a concat is added to a concat, then we can just inline the elements of
    // the child concat.
    if (unit->tag() == SimpleQuery::Unit::Tag::CONCAT) {
      for (auto& child : unit->drain_children()) {
        concat.add_child(child);
      }
    } else {
      concat.add_child(unit);
    }
  }
  static void add_to_alternation(SimpleQuery::Unit& alt, SimpleUnitPtr unit) {
    // If an alt is added to an alt, then we can add all alternatives of the
    // child alt instead.
    if (unit->tag() == SimpleQuery::Unit::Tag::ALTERNATION) {
      for (auto& child : unit->drain_children()) {
        alt.add_child(child);
      }
    } else {
      alt.add_child(unit);
    }
  }

private:
  SimpleUnitPtr plus_to_simple(const UnitPtr& unit) {
    const auto plus = SimpleQuery::Unit::new_concat(unit);
    plus->add_child(SimpleQuery::Unit::new_qmark(unit));
    plus->add_child(SimpleQuery::Unit::new_star(unit));
    return plus;
  }

  SimpleUnitPtr regex_to_simple(const UnitPtr& unit) {
    if (allow_regex) {
      return SimpleQuery::Unit::new_regex(unit->text(), unit);
    } else {
      // regexes are not allowed -> replace them with the empty alternation
      return SimpleQuery::Unit::new_alternation(unit);
    }
  }

  SimpleUnitPtr dict_to_simple(const UnitPtr& unit) {
    const std::string& text = unit->text();
    const auto text_unit = SimpleQuery::Unit::new_word(text, unit);

    if (!dictionary) {
      // no dictionary for this index
      return text_unit;
    }

    const auto range = dictionary->equal_range(text);
    if (range.first == range.second) {
      // the word isn't in the dictionary, hence no synonyms
      return text_unit;
    }

    // make an alternation with the text itself and all of its synonyms
    // (be sure to ignore the original word just in case)

    const auto alt = SimpleQuery::Unit::new_alternation(unit);
    alt->add_child(text_unit);

    std::vector<std::string> words; // just a temporary buffer

    for (auto it = range.first; it != range.second; it++) {
      const auto& syn = it->second;
      if (syn == text) {
        continue;
      }

      // synonyms may be phrases, so e.g. `#fail` might map to "break down", so
      // we have to split the synonym into words first
      words.clear();
      boost::split(words, text, [](char c) { return c == ' '; });

      if (words.size() == 1) {
        // the synonym is just one word
        alt->add_child(SimpleQuery::Unit::new_word(words[0], unit));
      } else {
        // the synonym is a phrase
        const auto concat = SimpleQuery::Unit::new_concat(unit);
        for (const auto& word : words) {
          concat->add_child(SimpleQuery::Unit::new_word(word, unit));
        }
        alt->add_child(concat);
      }
    }

    return alt;
  }

  SimpleUnitPtr option_to_simple(const UnitPtr& unit) {
    const auto option = SimpleQuery::Unit::new_alternation(unit);

    if (unit->children().size() == 1) {
      // If an option set has 1 element, the option set make that element
      // optional. E.g. `[ foo ]` == `foo | EMPTY_CONCAT`
      option->add_child(SimpleQuery::Unit::new_concat(unit));
    }

    for (const auto& child : unit->children()) {
      add_to_alternation(*option, unit_to_simple(child));
    }
    return option;
  }

  SimpleUnitPtr order_to_simple(const UnitPtr& unit) {
    // Order sets easily blow up simple queries because an order set with n
    // element will be converted to (n+1)! elements (n! copies of the original n
    // elements + n! concat elements). Some blow-up is necessary, because that's
    // how the order set works, but the normalization options to prevent
    // malicious queries that could crash or block our server.
    //
    // 1) options.max_length:
    // We know that all generated concats have the same min_length (because
    // min_length doesn't depend on the order of the elements in the concat), so
    // if a concat of all elements of the order set has a min_length >
    // options.max_length, then we know that all norm queries generated from
    // this order set will be discarded anyway. Thus we don't need to do any
    // work and can just return an empty alternation.
    //
    // 2) options.max_norm_queries:
    // We know that we will create n! concats, so have to assume all of them
    // will translate into norm queries, creating n! many. If n! >
    // options.max_norm_queries, we can abort early because the input query will
    // (very likely) create too many norm queries.

    std::vector<SimpleUnitPtr> elements;
    uint32_t elements_min_length = 0;
    for (const auto& child : unit->children()) {
      const auto child_simple = unit_to_simple(child);
      const auto range = child_simple->length_range();
      if (range.empty()) {
        // any concat with a unit that doesn't accept any phrases will also not
        // accept any phrases
        return SimpleQuery::Unit::new_alternation(unit);
      } else {
        elements_min_length += range.min;
      }

      if (range.max == 0) {
        // the child is equal to the empty concat and can therefore be removed
        continue;
      }
      elements.push_back(unit_to_simple(child));
    }

    // check 1)
    if (elements_min_length > options.max_length) {
      return SimpleQuery::Unit::new_alternation(unit);
    }

    // check 2)
    if (elements.size() >= 7 ||
        util::factorial(elements.size()) > options.max_norm_queries) {
      // 7! will result in at least 7! (~5k) norm queries and at least 8! (~40k)
      // elements, so it's used as a hard limit. Our current indexes only
      // support up to 5-grams, so this shouldn't be an issue.

      throw invalid_query_error("Orderset with too many options.");
    }

    // Special cases for 0 and 1 many elements.

    if (elements.size() == 0) {
      // A empty order set is equal to the empty concat.
      return SimpleQuery::Unit::new_concat(unit);
    }
    if (elements.size() == 1) {
      // An order set with one unit is equal to its unit.
      return elements[0];
    }

    // This is the general case.
    // Make an alternative that contains the all permutations.

    const auto alt = SimpleQuery::Unit::new_alternation(unit);

    std::vector<size_t> indexes;
    for (size_t i = 0; i < elements.size(); i++) {
      indexes.push_back(i);
    }

    // go through all permutations of indexes

    // All the permutating is done by std::next_permutation.
    // https://en.cppreference.com/w/cpp/algorithm/next_permutation
    do {
      const auto perm_concat = SimpleQuery::Unit::new_concat(unit);

      for (const auto i : indexes) {
        add_to_concat(*perm_concat, elements[i]->clone());
      }

      alt->add_child(perm_concat);
    } while (std::next_permutation(indexes.begin(), indexes.end()));

    return alt;
  }

  SimpleUnitPtr alt_to_simple(const UnitPtr& unit) {
    const auto alt = SimpleQuery::Unit::new_alternation(unit);
    for (const auto& child : unit->children()) {
      add_to_alternation(*alt, unit_to_simple(child));
    }
    return alt;
  }

  SimpleUnitPtr concat_to_simple(const UnitPtr& unit) {
    const auto concat = SimpleQuery::Unit::new_concat(unit);
    for (const auto& child : unit->children()) {
      add_to_concat(*concat, unit_to_simple(child));
    }
    return concat;
  }

  SimpleUnitPtr unit_to_simple(const UnitPtr& unit) {
    switch (unit->tag()) {
      case Query::Unit::Tag::WORD:
        return SimpleQuery::Unit::new_word(unit->text(), unit);

      case Query::Unit::Tag::QMARK:
        return SimpleQuery::Unit::new_qmark(unit);
      case Query::Unit::Tag::STAR:
        return SimpleQuery::Unit::new_star(unit);
      case Query::Unit::Tag::PLUS:
        return plus_to_simple(unit);

      case Query::Unit::Tag::REGEX:
        return regex_to_simple(unit);
      case Query::Unit::Tag::DICTSET:
        return dict_to_simple(unit);

      case Query::Unit::Tag::OPTIONSET:
        return option_to_simple(unit);
      case Query::Unit::Tag::ORDERSET:
        return order_to_simple(unit);

      case Query::Unit::Tag::ALTERNATION:
        return alt_to_simple(unit);
      case Query::Unit::Tag::CONCAT:
        return concat_to_simple(unit);

      default:
        throw std::logic_error("Unknown tag");
    }
  }

public:
  std::unique_ptr<SimpleQuery> to_simple(const Query& query) {
    return std::make_unique<SimpleQuery>(unit_to_simple(query.alternatives()));
  }
};

/**
 * @brief A class that compute the length ranges of given units can caches the
 * results.
 */
class LengthRangeCacher {
private:
  std::unordered_map<std::shared_ptr<const SimpleQuery::Unit>, uint32_t> cache_;

  uint32_t compute_min_length(const SimpleQuery::Unit& unit) {
    switch (unit.tag()) {
      case SimpleQuery::Unit::Tag::STAR:
        return 0;

      case SimpleQuery::Unit::Tag::WORD:
      case SimpleQuery::Unit::Tag::QMARK:
      case SimpleQuery::Unit::Tag::REGEX:
        return 1;

      case SimpleQuery::Unit::Tag::ALTERNATION: {
        uint32_t min = UINT32_MAX;
        for (const auto& child : unit.children()) {
          const auto c_min = get_min_length(child);
          if (c_min < min) {
            if (c_min == 0) {
              return 0;
            }
            min = c_min;
          }
        }
        return min;
      }

      case SimpleQuery::Unit::Tag::CONCAT: {
        uint32_t min = 0;
        for (const auto& child : unit.children()) {
          const auto c_min = get_min_length(child);
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

public:
  uint32_t get_min_length(
      const std::shared_ptr<const SimpleQuery::Unit>& unit) {
    switch (unit->tag()) {
      case SimpleQuery::Unit::Tag::WORD:
      case SimpleQuery::Unit::Tag::QMARK:
      case SimpleQuery::Unit::Tag::REGEX:
        return 1;
      case SimpleQuery::Unit::Tag::STAR:
        return 0;
      default:
        break;
    }

    // only concats and alts have to be changed

    const auto res = cache_.find(unit);
    if (res == cache_.end()) {
      // compute and cache
      const auto min = unit->min_length();
      cache_.emplace(unit, min);
      return min;
    } else {
      return res->second;
    }
  }
};

/**
 * @brief A min length margin represents the minimum length it takes to reach a
 * unit and the minimum length it takes to go to the end of the query from that
 * unit.
 *
 */
struct MinLengthMargin {
public:
  uint32_t before;
  uint32_t after;

  uint32_t total() const {
    return before + after;
  }

  MinLengthMargin() = delete;
  MinLengthMargin(uint32_t before, uint32_t after)
      : before(before), after(after){};
};

MinLengthMargin operator+(const MinLengthMargin& lhs,
                          const MinLengthMargin& rhs) {
  return MinLengthMargin(lhs.before + rhs.before, lhs.after + rhs.after);
}


class SimpleQueryOptimizer {
private:
  // A few general things about the optimizations and how they are implemented:
  //
  // The optimizer tries to minimize the number of units in the tree by applying
  // trivial optimizations like inlining nested concatentation and nested
  // alternations.
  //
  // During these optimizations, units are only allowed to replace themselves
  // with exactly one element. They **are not** allowed to remove themselves or
  // any other child of any of their ancestors. They are also not allowed to
  // replace themselves with more than one unit or to add units to any of their
  // ancestors.

  /**
   * @brief This will optimize the given unit.
   *
   * Note: This method assumes that the given unit has a parent!
   *
   * @param unit
   */
  void optimize_simple_unit(SimpleQuery::Unit& unit) const {
    // the only thing to optimize are alternations and concatenations
    if (unit.tag() == SimpleQuery::Unit::Tag::CONCAT) {
      optimize_simple_concat(unit);
    } else if (unit.tag() == SimpleQuery::Unit::Tag::ALTERNATION) {
      optimize_simple_alt(unit);
    }
  }

  void optimize_simple_children(SimpleQuery::Unit& unit) const {
    const auto& children = unit.children();
    for (size_t i = 0; i < children.size(); i++) {
      optimize_simple_unit(*children[i]);
    }
  }

  static bool contains_child(const SimpleQuery::Unit& unit,
                             const SimpleQuery::Unit::Tag tag) {
    return std::any_of(unit.children().begin(), unit.children().end(),
                       [&tag](std::shared_ptr<SimpleQuery::Unit> u) {
                         return u->tag() == tag;
                       });
  }

  static void inline_nested(SimpleQuery::Unit& unit,
                            const SimpleQuery::Unit::Tag tag) {
    // check if there are nested units to inline
    if (contains_child(unit, tag)) {
      const auto old = unit.drain_children();
      for (const auto& u : old) {
        if (u->tag() == tag) {
          for (const auto nested_child : u->drain_children()) {
            unit.add_child(nested_child);
          }
        } else {
          unit.add_child(u);
        }
      }
    }
  }

  void optimize_simple_concat(SimpleQuery::Unit& unit) const {
    optimize_simple_children(unit);

    if (unit.children().empty()) {
      // empty concats are removed by the parent
      return;
    }

    if (unit.children().size() == 1) {
      // replace the one-children concat with its child
      const auto child = unit.children()[0];
      unit.clear_children(); // detach child from tree
      unit.replace_with(child);
      return;
    }

    inline_nested(unit, SimpleQuery::Unit::Tag::CONCAT);

    // check for the empty alternation
    // The empty alternation is the absorbing element of concatenation
    const auto empty =
        std::find_if(unit.children().begin(), unit.children().end(),
                     [](std::shared_ptr<SimpleQuery::Unit> u) {
                       return u->tag() == SimpleQuery::Unit::Tag::ALTERNATION &&
                              u->children().empty();
                     });
    if (empty != unit.children().end()) {
      const auto empty_alt = *empty;
      unit.clear_children(); // detach children from tree
      unit.replace_with(empty_alt);
      return;
    }

    // Star optimization
    //
    // The basic idea here is that the star operator has the power to "absorb"
    // other units. I.e. the star in `the * [ foo ]` can absorb the optional
    // word "foo" meaning that this query is equivalent to `the *`.
    //
    // More generally, the star can absorb any adjacent unit with a minimum
    // length of 0 (e.g. optional word, other stars). The limiting factor then
    // becomes the "adjacent" condition but we can switch the order of stars and
    // QMarks, so we can potentially absorb even more units. E.g. `* ? [ foo ]`
    // == `? * [ foo ]` == `? *` == `* ?`.
    //
    // Note: The relative order child units in concatentation is not actually
    // changed. This is important because the source backtracking can become
    // very confusing otherwise (with units unexpectedly changing positions).

    if (!contains_child(unit, SimpleQuery::Unit::Tag::STAR)) {
      // without a star, there's nothing to optimize
      return;
    }

    std::vector<std::shared_ptr<SimpleQuery::Unit>> to_remove;
    for (size_t i = 0; i < unit.children().size(); i++) {
      const auto& star = unit.children()[i];
      if (star->tag() != SimpleQuery::Unit::Tag::STAR) {
        // not a star
        continue;
      }
      // we don't have to worry about this being a absorbed star (e.g. `* * *`
      // == `*`) because we skip already processed elements in the lookahead

      // at this point in the optimization method, we already optimized a few
      // things, so we don't have to handle them here. Stuff like an empty
      // alternation absorbing everything or inlining nested concats.

      // lookbehind
      for (size_t j = i; j > 0; j--) {
        const auto& child = unit.children()[j - 1];
        if (child->tag() == SimpleQuery::Unit::Tag::QMARK) {
          continue;
        }
        if (child->min_length() == 0) {
          to_remove.push_back(child);
          continue;
        }
        // Note: This is not sufficent. The child might be an alternation in
        // which case, it might have an alternative that, in the case of this
        // lookbehind, ends with units that can be removed. E.g. `( a | b * ) *`
        // == `( a | b ) *`. Similar for the lookahead. This isn't implement,
        // because with the current query syntax, it's not possible to create
        // such an AST.
        break;
      }

      // lookahead and skip this number of elements
      for (size_t j = i + 1; j < unit.children().size(); j++) {
        const auto& child = unit.children()[j];
        if (child->tag() == SimpleQuery::Unit::Tag::QMARK) {
          continue;
        }
        if (child->min_length() == 0) {
          to_remove.push_back(child);
          continue;
        }
        i = j; // skip already processed elements
        break;
      }
    }

    // remove the flagged units
    // Note: We don't remove these units while iterating over unit.children()
    // because that will only cause problems
    for (const auto& rem : to_remove) {
      rem->remove();
    }
  }

  void optimize_simple_alt(SimpleQuery::Unit& unit) const {
    optimize_simple_children(unit);

    if (unit.children().empty()) {
      // empty alt are removed by the parent
      return;
    }

    if (unit.children().size() == 1) {
      // replace the one-children alt with its child
      const auto child = unit.children()[0];
      unit.clear_children(); // detach child from tree
      unit.replace_with(child);
      return;
    }

    inline_nested(unit, SimpleQuery::Unit::Tag::ALTERNATION);

    // Right now, the Netspeak Query syntax is simple enough that further
    // optimization is unnecessary. Should we extend the query syntax or allow
    // users to send us query ASTs directly, this should be updated to include
    // more optimizations.
    //
    // One such optimization is based of min and max lengths of paths. We can
    // eliminate certain alternatives
  }

public:
  void optimize(SimpleQuery& query) const {
    optimize_simple_children(*query.root());
  }
};

struct NormQueryCounter {
public:
  QueryNormalizer::Options options;

  NormQueryCounter(const QueryNormalizer::Options& options)
      : options(options) {}

  /**
   * @brief Returns an estimate for the number of norm queries creates by the
   * given query.
   *
   * The returned estimate is guaranteed to be always higher than the actual
   * number of norm queries (but also bounded by the maximum number
   * representable in \c uint64_t ).
   *
   * Every regex in the given query will be assumed to be replaced by the given
   * number of words.
   *
   * @param query
   * @param regex_matches
   */
  uint64_t estimate(const SimpleQuery& query, size_t regex_matches) {
    LengthRangeCacher cacher;
    MinLengthMargin margin(0, 0);
    return estimate_impl(query.root(), regex_matches, cacher, margin);
  }

private:
  uint64_t estimate_impl(const std::shared_ptr<const SimpleQuery::Unit>& unit,
                         size_t regex_matches, LengthRangeCacher& cacher,
                         const MinLengthMargin margin) {
    switch (unit->tag()) {
      case SimpleQuery::Unit::Tag::WORD:
      case SimpleQuery::Unit::Tag::QMARK:
        return 1;

      case SimpleQuery::Unit::Tag::STAR:
        return 1UL + std::max<uint32_t>(0, options.max_length - margin.total());

      case SimpleQuery::Unit::Tag::REGEX:
        return regex_matches;

      case SimpleQuery::Unit::Tag::ALTERNATION: {
        uint64_t sum = 0;
        for (const auto& child : unit->children()) {
          const uint64_t c_num =
              estimate_impl(child, regex_matches, cacher, margin);
          if (c_num == UINT64_MAX) {
            return UINT64_MAX;
          }
          sum += c_num;
        }
        return sum;
      }

      case SimpleQuery::Unit::Tag::CONCAT: {
        uint64_t total = 1;

        const uint32_t unit_min_len = cacher.get_min_length(unit);
        if (unit_min_len == UINT32_MAX) {
          return 0;
        }

        uint32_t min_before = 0;
        for (const auto& child : unit->children()) {
          const uint32_t child_min = cacher.get_min_length(child);
          const uint32_t min_after = unit_min_len - min_before - child_min;
          const uint64_t c_num =
              estimate_impl(child, regex_matches, cacher,
                            margin + MinLengthMargin(min_before, min_after));
          min_before += child_min;

          if (c_num == UINT64_MAX) {
            return UINT64_MAX;
          }
          if (c_num == 0) {
            return 0;
          }
          const uint64_t prod = c_num * total;
          if (prod < c_num || prod < total) {
            // an overflow occurred
            return UINT64_MAX;
          }
          total = prod;
        }
        return total;
      }

      default:
        throw std::logic_error("Unknown tag");
    }
  }
};

const std::shared_ptr<const std::string> QMARK_STR =
    std::make_shared<const std::string>("?");
const std::shared_ptr<const std::string> STAR_STR =
    std::make_shared<const std::string>("*");
/**
 * @brief A star queries is basically the same as a norm query but it can still
 * contain stars.
 */
class StarQuery {
public:
  struct Unit {
  public:
    enum class Tag { WORD, QMARK, STAR };
    typedef std::shared_ptr<const std::string> Text;
    typedef std::shared_ptr<const Query::Unit> Source;

    Tag tag;
    Text text;
    Source source;

    Unit() = delete;
    Unit(Tag tag, const Text& text, const Source& source)
        : tag(tag), text(text), source(source) {}

    static Unit new_word(const Text& text, const Source& source) {
      return Unit(Tag::WORD, text, source);
    }
    static Unit new_qmark(const Source& source) {
      return Unit(Tag::QMARK, QMARK_STR, source);
    }
    static Unit new_star(const Source& source) {
      return Unit(Tag::STAR, STAR_STR, source);
    }
  };

private:
  std::vector<Unit> units_;
  uint32_t min_length_;

public:
  const std::vector<Unit>& units() const {
    return units_;
  }

  uint32_t min_length() const {
    return min_length_;
  }
  uint32_t max_length() const {
    uint32_t max = 0;
    for (const auto& unit : units()) {
      if (unit.tag == Unit::Tag::STAR) {
        return UINT32_MAX;
      } else {
        max++;
      }
    }
    return max;
  }

  void push_back(const Unit& unit) {
    if (unit.tag != Unit::Tag::STAR) {
      min_length_++;
    }
    units_.push_back(unit);
  }
  void append(const StarQuery& query) {
    min_length_ += query.min_length_;
    util::vec_append(units_, query.units_);
  }
};

class StarQueryConverter {
public:
  QueryNormalizer::Options options;

  StarQueryConverter() = delete;
  StarQueryConverter(const QueryNormalizer::Options& options)
      : options(options) {}

  std::vector<StarQuery> to_star_queries(const SimpleQuery& query) {
    std::vector<StarQuery> result;
    alternation(result, *query.root());
    return result;
  }

private:
  void concat(std::vector<StarQuery>& queries, const SimpleQuery::Unit& unit) {
    switch (unit.tag()) {
      case SimpleQuery::Unit::Tag::WORD: {
        const auto u = StarQuery::Unit::new_word(
            std::make_shared<const std::string>(unit.text()), unit.source());
        for (auto& q : queries) {
          q.push_back(u);
        }
        break;
      }
      case SimpleQuery::Unit::Tag::QMARK: {
        const auto u = StarQuery::Unit::new_qmark(unit.source());
        for (auto& q : queries) {
          q.push_back(u);
        }
        break;
      }
      case SimpleQuery::Unit::Tag::STAR: {
        const auto u = StarQuery::Unit::new_star(unit.source());
        for (auto& q : queries) {
          q.push_back(u);
        }
        break;
      }
      case SimpleQuery::Unit::Tag::REGEX: {
        throw std::logic_error("Regexes should have already been handled.");
      }
      case SimpleQuery::Unit::Tag::ALTERNATION: {
        std::vector<StarQuery> alt;
        alternation(alt, unit);

        if (alt.empty()) {
          queries.clear();
        } else if (alt.size() == 1) {
          const auto& alt_q = alt[0];
          for (auto& q : queries) {
            q.append(alt_q);
          }
        } else {
          const std::vector<StarQuery> copy(queries);

          queries.clear();
          queries.reserve(copy.size() * alt.size());
          for (const auto& before : copy) {
            for (const auto& after : alt) {
              if (before.min_length() + after.min_length() >
                  options.max_length) {
                // This is where a combinatory explosion takes place, so we try
                // to optimize this case as much as possible by discarding star
                // queries that are too long.
                continue;
              }
              StarQuery q;
              q.append(before);
              q.append(after);
              queries.push_back(std::move(q));
            }
          }
        }
        break;
      }
      case SimpleQuery::Unit::Tag::CONCAT: {
        for (const auto& child : unit.children()) {
          concat(queries, *child);
        }
        break;
      }
      default:
        throw std::logic_error("Unknown Tag");
    }
  }
  void alternation(std::vector<StarQuery>& queries,
                   const SimpleQuery::Unit& unit) {
    switch (unit.tag()) {
      case SimpleQuery::Unit::Tag::WORD: {
        StarQuery q;
        q.push_back(StarQuery::Unit::new_word(
            std::make_shared<const std::string>(unit.text()), unit.source()));
        queries.push_back(std::move(q));
        break;
      }
      case SimpleQuery::Unit::Tag::QMARK: {
        StarQuery q;
        q.push_back(StarQuery::Unit::new_qmark(unit.source()));
        queries.push_back(std::move(q));
        break;
      }
      case SimpleQuery::Unit::Tag::STAR: {
        StarQuery q;
        q.push_back(StarQuery::Unit::new_star(unit.source()));
        queries.push_back(std::move(q));
        break;
      }
      case SimpleQuery::Unit::Tag::REGEX: {
        throw std::logic_error("Regexes should have already been handled.");
      }
      case SimpleQuery::Unit::Tag::ALTERNATION: {
        for (const auto& child : unit.children()) {
          alternation(queries, *child);
        }
        break;
      }
      case SimpleQuery::Unit::Tag::CONCAT: {
        std::vector<StarQuery> con;
        con.push_back(StarQuery());
        concat(con, unit);
        for (const auto& q : con) {
          if (q.min_length() <= options.max_length) {
            // we go through all queries anyway, so we might as well check the
            // min length
            queries.push_back(q);
          }
        }
        break;
      }
      default:
        throw std::logic_error("Unknown Tag");
    }
  }
};

class SimpleQueryNormalizer {
public:
  QueryNormalizer::Options options;

  SimpleQueryNormalizer() = delete;
  SimpleQueryNormalizer(const QueryNormalizer::Options& options)
      : options(options) {}

  /**
   * @brief Converts the given simple query into norm queries and adds them to
   * the given list of norm queries.
   *
   * All norm queries added are guaranteed to fulfill the min length and max
   * length requirements set by the normalizer options. If the number of norm
   * queries exceeds the maximum set by the normalizer options, an exception
   * will be thrown.
   *
   * @param norm_queries
   * @param query
   */
  void to_norm_queries(std::vector<NormQuery>& norm_queries,
                       const SimpleQuery& query) {
    StarQueryConverter star_conv(options);
    std::vector<StarQuery> star_queries = star_conv.to_star_queries(query);

    for (const auto& q : star_queries) {
      const auto min = q.min_length();
      const auto max = q.max_length();
      // this means that the star query does not contain stars, so it's a valid
      // norm query
      if (min <= options.max_length && min >= options.min_length) {
        if (min == max) {
          add_simple(norm_queries, q);
        } else {
          add_with_stars(norm_queries, q);
        }
      }

      if (norm_queries.size() > options.max_norm_queries) {
        throw invalid_query_error("Too many norm queries");
      }
    }
  }

private:
  void add_simple(std::vector<NormQuery>& norm_queries,
                  const StarQuery& query) {
    NormQuery nq;
    for (const auto& u : query.units()) {
      switch (u.tag) {
        case StarQuery::Unit::Tag::WORD:
          nq.units().push_back(NormQuery::Unit::word(u.text, u.source));
          break;
        case StarQuery::Unit::Tag::QMARK:
          nq.units().push_back(NormQuery::Unit::qmark(u.source));
          break;
        default:
          throw std::logic_error("Star is not valid here.");
      }
    }
    norm_queries.push_back(nq);
  }
  void add_with_stars(std::vector<NormQuery>& norm_queries,
                      const StarQuery& query) {
    // the number of stars in the query
    const size_t star_count =
        std::count_if(query.units().begin(), query.units().end(),
                      [](const StarQuery::Unit& u) {
                        return u.tag == StarQuery::Unit::Tag::STAR;
                      });
    // the available space to expand stars.
    const size_t available = options.max_length - query.min_length();

    util::ChainCutter cutter(star_count, available);
    // The chain cutter will go through all possible cuts on a chain of a
    // certain length. We choose the length of the chain to be our available
    // space and the number of cuts to the number of stars. This means that the
    // first available-many segments of each iteration will tell us with how
    // many qMarks we have to replace each star with.

    do {
      NormQuery nq;
      size_t star_i = 0;
      for (const auto& u : query.units()) {
        switch (u.tag) {
          case StarQuery::Unit::Tag::WORD:
            nq.units().push_back(NormQuery::Unit::word(u.text, u.source));
            break;
          case StarQuery::Unit::Tag::QMARK:
            nq.units().push_back(NormQuery::Unit::qmark(u.source));
            break;
          case StarQuery::Unit::Tag::STAR: {
            size_t qmark_count = cutter.segments()[star_i++];
            for (; qmark_count != 0; qmark_count--) {
              nq.units().push_back(NormQuery::Unit::qmark(u.source));
            }
            break;
          }
          default:
            throw std::logic_error("Unknown tag");
        }
      }
      if (nq.size() >= options.min_length) {
        // the query might be too short, so we have to check
        norm_queries.push_back(nq);
      }
    } while (cutter.next());
  }
};

void find_regexes(const std::shared_ptr<SimpleQuery::Unit>& unit,
                  std::vector<std::shared_ptr<SimpleQuery::Unit>>& regexes) {
  switch (unit->tag()) {
    case SimpleQuery::Unit::Tag::REGEX:
      regexes.push_back(unit);
      break;
    case SimpleQuery::Unit::Tag::ALTERNATION:
    case SimpleQuery::Unit::Tag::CONCAT:
      for (const auto& child : unit->children()) {
        find_regexes(child, regexes);
      }
      break;
    default:
      break;
  }
}
std::vector<std::shared_ptr<SimpleQuery::Unit>> find_regexes(
    SimpleQuery& query) {
  std::vector<std::shared_ptr<SimpleQuery::Unit>> regexes;
  find_regexes(query.root(), regexes);
  return regexes;
}

/**
 * @brief Finds the best possible value in the range \c min to \c max (both
 * inclusive) such that \c comparer(value)<=0 .
 *
 * @param min
 * @param max
 * @param comparer
 */
size_t binary_search(size_t min, size_t max,
                     std::function<int(size_t)> comparer) {
  while (min < max) {
    size_t m = min + ((max - min) >> 1) + 1;
    int cmp = comparer(m);
    if (cmp == 0) {
      return m;
    } else if (cmp < 0) {
      min = m;
    } else {
      max = m - 1;
    }
  }
  return min;
}

void normalize_regexes(SimpleQuery& query,
                       const QueryNormalizer::Options& options,
                       const std::shared_ptr<const RegexIndex>& regex_index) {
  const auto regexes = find_regexes(query);
  if (!regexes.empty()) {
    // try to find out with how many words we can replace each regex and still
    // fall within our budget.
    // It's like Blackjack - try to go as high as possible but don't go over.
    NormQueryCounter counter(options);

    // This comparer will return <= 0 if the number of regexes matches fall
    // which our budget and > 0 other wise.
    auto comparer = [&](size_t m) {
      size_t estimate = counter.estimate(query, m);
      if (estimate == options.max_norm_queries) {
        return 0;
      } else {
        return estimate < options.max_norm_queries ? -1 : 1;
      }
    };

    // the maximum number of regex words such that we still fall <= our budget
    size_t best_max_matches;
    if (comparer(options.max_regex_matches) <= 0) {
      // it's quite likely that this will be the case
      best_max_matches = options.max_regex_matches;
    } else {
      best_max_matches = binary_search(0, options.max_regex_matches, comparer);
    }

    if (best_max_matches == 0) {
      // By our estimate, we can't even replace the regexes with a single word
      // without going over the budget.

      // TODO: What now?
    } else {
      // TODO: Parallelize the regex matching

      std::vector<std::string> matches;
      for (const auto& regex_unit : regexes) {
        const auto regex_query =
            regex::parse_netspeak_regex_query(regex_unit->text());
        regex_index->match_query(regex_query, matches, best_max_matches,
                                 options.max_regex_time);

        const auto source = regex_unit->source();
        const auto alt = SimpleQuery::Unit::new_alternation(source);
        for (const auto& word : matches) {
          alt->add_child(SimpleQuery::Unit::new_word(word, source));
        }
        regex_unit->replace_with(alt);

        matches.clear();
      }
    }
  }
}

void QueryNormalizer::normalize(const Query& query, const Options& options,
                                std::vector<NormQuery>& norm_queries) {
  const auto query_length_range = query.length_range();
  if (query_length_range.empty() ||
      query_length_range.max < options.min_length ||
      query_length_range.min > options.max_length) {
    // In this case, we just cannot create any norm queries that fulfill the
    // normalization options.
    return;
  }

  bool allow_regex = !!regex_index_ && options.max_regex_matches > 0 &&
                     options.max_regex_time > std::chrono::nanoseconds(0);

  // convert the given query into a simpler form
  QuerySimplifier simplifier(options);
  simplifier.dictionary = dictionary_;
  simplifier.allow_regex = allow_regex;
  const std::unique_ptr<SimpleQuery> simple = simplifier.to_simple(query);

  // optimize the simple form
  SimpleQueryOptimizer optimizer;
  optimizer.optimize(*simple);

  // replaces all regexes with a list of words
  normalize_regexes(*simple, options, regex_index_);

  // create the norm queries
  SimpleQueryNormalizer normalizer(options);
  normalizer.to_norm_queries(norm_queries, *simple);
}

QueryNormalizer::QueryNormalizer(const InitConfig& config)
    : regex_index_(config.regex_index), dictionary_(config.dictionary) {}

} // namespace netspeak
