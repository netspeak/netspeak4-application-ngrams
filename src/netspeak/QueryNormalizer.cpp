#include "netspeak/QueryNormalizer.hpp"

#include <algorithm>

#include "boost/algorithm/string.hpp"

#include "netspeak/internal/SimpleQuery.hpp"
#include "netspeak/util/Orderings.hpp"


namespace netspeak {

/**
 * @brief A simple function that appends all elements of \c other to \c base.
 *
 * @tparam T
 * @param base
 * @param other
 */
template <typename T>
inline void vec_append(std::vector<T>& base, const std::vector<T>& other) {
  base.insert(base.end(), other.begin(), other.end());
}

size_t fact(size_t n) {
  // BE WARE OF OVERFLOWS!
  // This will return 0 for n>33 on 32bit or n>65 for 64bit systems
  size_t total = 1;
  for (; n > 1; n--) {
    total *= n;
  }
  return total;
}

class NormQueryCollection {
  std::vector<NormQuery> norm_queries_;
  size_t max_length_;

public:
  NormQueryCollection() = delete;
  NormQueryCollection(size_t max_length) : max_length_(max_length){};
  NormQueryCollection(const NormQueryCollection&) = delete;

  bool empty() const {
    return norm_queries_.empty();
  }
  size_t size() const {
    return norm_queries_.size();
  }
  size_t max_length() const {
    return max_length_;
  }
  const std::vector<NormQuery>& norm_queries() const {
    return norm_queries_;
  }

  void push_back(const NormQuery& query) {
    if (query.size() <= max_length_) {
      norm_queries_.push_back(query);
    }
  }
  void push_back(NormQuery&& query) {
    if (query.size() <= max_length_) {
      norm_queries_.push_back(std::move(query));
    }
  }

  void append(std::unique_ptr<NormQueryCollection> other) {
    if (norm_queries_.empty()) {
      // concat(EMPTY, other) == EMPTY
      return;
    }
    if (other->empty()) {
      // concat(this, EMPTY) == EMTPY
      norm_queries_.clear();
      return;
    }
    if (norm_queries_.size() == 1 && norm_queries_[0].units().empty()) {
      // concat([EMPTY_NORM_QUERY], other) == other
      std::swap(norm_queries_, other->norm_queries_);
      return;
    }
    if (other->size() == 1) {
      // no need to make a copy. Just append the one query to all
      const auto& other_units = other->norm_queries_[0].units();
      if (other_units.empty()) {
        // concat(this, [EMPTY_NORM_QUERY]) == this
        return;
      }

      for (auto& q : norm_queries_) {
        vec_append(q.units(), other_units);
      }
      return;
    }

    // make a copy of the original
    const std::vector<NormQuery> original(norm_queries_);
    const auto& other_queries = other->norm_queries_;

    norm_queries_.clear();
    norm_queries_.reserve(original.size() * other_queries.size());

    for (const auto& this_q : original) {
      for (const auto& other_q : other_queries) {
        auto query = NormQuery();
        const size_t length = this_q.size() + other_q.size();
        if (length > max_length()) {
          continue;
        }
        query.units().reserve(length);
        vec_append(query.units(), this_q.units());
        vec_append(query.units(), other_q.units());
        norm_queries_.push_back(std::move(query));
      }
    }
  }

  void alternation(std::unique_ptr<NormQueryCollection> other) {
    if (norm_queries_.empty()) {
      // union(EMPTY, other) == other
      std::swap(norm_queries_, other->norm_queries_);
      return;
    }
    if (other->empty()) {
      // union(this, EMPTY) == this
      return;
    }

    if (other->max_length_ <= max_length_) {
      vec_append(norm_queries_, other->norm_queries_);
    } else {
      for (const auto& query : other->norm_queries_) {
        if (query.size() <= max_length_) {
          norm_queries_.push_back(query);
        }
      }
    }
  }
};

/**
 * @brief A converter from a \c Query to its \c SimpleQuery units.
 */
struct QuerySimplifier {
public:
  QueryNormalizer::Options options;
  std::shared_ptr<const Dictionaries::Map> dictionary;

  QuerySimplifier() = delete;
  QuerySimplifier(const QueryNormalizer::Options& options) : options(options) {}

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
      const auto min_length = child_simple->min_length();
      if (min_length == UINT32_MAX) {
        // any concat with a unit that doesn't accept any phrases will also not
        // accept any phrases
        return SimpleQuery::Unit::new_alternation(unit);
      } else {
        elements_min_length += min_length;
      }

      if (child_simple->max_length() == 0) {
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
        fact(elements.size()) > options.max_norm_queries) {
      // 7! will result in at least 7! (~5k) norm queries and at least 8! (~40k)
      // elements, so it's used as a hard limit. Our current indexes only
      // support up to 5-grams, so this shouldn't be an issue.

      // TODO: Choose an appropriate error type
      throw std::runtime_error("Query too complex.");
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
        return SimpleQuery::Unit::new_regex(unit->text(), unit);
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
  std::shared_ptr<SimpleQuery> to_simple(const Query& query) {
    return std::make_shared<SimpleQuery>(
        new SimpleQuery(unit_to_simple(query.alternatives())));
  }
};


void QueryNormalizer::normalize(const Query& query, const Options& options,
                                std::vector<NormQuery>& norm_queries) {
  if (query.max_length() < options.min_length ||
      query.min_length() > options.max_length) {
    // In this case, we just cannot create any norm queries that fulfill the
    // normalization options.
    return;
  }

  QuerySimplifier simplifier(options);
  simplifier.dictionary = dictionary_;
  const std::shared_ptr<SimpleQuery> simple = simplifier.to_simple(query);


  // TODO: implement
}

QueryNormalizer::QueryNormalizer(const InitConfig& config)
    : regex_index_(config.regex_index), dictionary_(config.dictionary) {}

} // namespace netspeak
