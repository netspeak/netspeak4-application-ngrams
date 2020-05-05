#ifndef NETSPEAK_QUERY_PROCESSOR_HPP
#define NETSPEAK_QUERY_PROCESSOR_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <unordered_set>
#include <vector>

#include "aitools/util/check.hpp"
#include "aitools/util/logging.hpp"

#include "netspeak/Configurations.hpp"
#include "netspeak/RetrievalStrategy.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/protobuf_utils.hpp"
#include "netspeak/query_methods.hpp"

namespace netspeak {

template <typename RetrievalStrategyTag> class QueryProcessor {
private:
  typedef RetrievalStrategy<RetrievalStrategyTag> retrieval_strategy;
  typedef typename retrieval_strategy::index_entry_type index_entry_type;
  typedef index_entry_traits<RetrievalStrategyTag> traits;

  /**
   * A hash function object for hashing index entries.
   */
  struct hash : public std::unary_function<index_entry_type, size_t> {
    size_t operator()(const index_entry_type& entry) const {
      return traits::hash(entry);
    }
  };

  /**
   * A comparator function object for comparing ngram references.
   */
  struct equal
      : std::binary_function<index_entry_type, index_entry_type, bool> {
    bool operator()(const index_entry_type& a,
                    const index_entry_type& b) const {
      return traits::equal(a, b);
    }
  };

  typedef std::unordered_set<index_entry_type, hash, equal>
      intersection_set_type;

public:
  void initialize(const Configurations::Map& config) {
    strategy_.initialize(config);
  }

  const Properties::Map properties() const {
    // Should return phrase_index and postlist_index
    // properties as defined in NetspeakProperties.hpp.
    return strategy_.properties();
  }

  std::shared_ptr<generated::QueryResult> process(
      const generated::Request& request, const generated::Query& query) {
    aitools::check(is_normalized(query), "is not normalized", query);
    return process_(request, query);
  }

private:
  static const std::vector<generated::Query::Unit> extract_sorted_words(
      const generated::Query& query) {
    std::vector<generated::Query::Unit> words;
    for (const auto& unit : query.unit()) {
      switch (unit.tag()) {
        case generated::Query::Unit::WORD:
        case generated::Query::Unit::WORD_IN_DICTSET:
        case generated::Query::Unit::WORD_FOR_REGEX:
        case generated::Query::Unit::WORD_IN_OPTIONSET:
        case generated::Query::Unit::WORD_IN_ORDERSET:
        case generated::Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET:
        case generated::Query::Unit::WORD_FOR_REGEX_IN_ORDERSET:
          words.push_back(unit);
          break;
        default:;
      }
    }
    std::sort(words.begin(), words.end());
    return words;
  }

  std::shared_ptr<generated::QueryResult> process_(
      const generated::Request& request, const generated::Query& query) {
    std::shared_ptr<generated::QueryResult> query_result(
        new generated::QueryResult());
    query_result->mutable_request()->CopyFrom(request);
    query_result->mutable_query()->CopyFrom(query);
    strategy_.initialize_query(request, *query_result->mutable_query());

    intersection_set_type src_set;
    intersection_set_type dst_set;
    intersection_set_type* src_set_ptr(&src_set);
    intersection_set_type* dst_set_ptr(&dst_set);

    std::vector<index_entry_type> index_entries;
    const auto query_words = extract_sorted_words(query_result->query());
    const auto last_word_it = std::prev(query_words.end());
    auto cur_max_phrase_frequency = request.max_phrase_frequency();
    for (auto it = query_words.begin(); it != query_words.end(); ++it) {
      if (it == query_words.begin()) {
        if (it == last_word_it) {
          const stats_type stats(strategy_.initialize_result_set(
              *it, query_result->query(), cur_max_phrase_frequency,
              request.max_phrase_count(), std::back_inserter(index_entries)));
          if (!stats.unknown_word.empty()) {
            query_result->add_unknown_word(stats.unknown_word);
          }
          query_result->set_union_size(query_result->union_size() +
                                       stats.eval_index_entry_count);
        } else {
          const stats_type stats(strategy_.initialize_result_set(
              *it, query_result->query(), cur_max_phrase_frequency,
              std::numeric_limits<size_t>::max(),
              std::inserter(*src_set_ptr, src_set_ptr->end())));
          cur_max_phrase_frequency = stats.max_phrase_frequency;
          if (!stats.unknown_word.empty()) {
            query_result->add_unknown_word(stats.unknown_word);
          }
          query_result->set_union_size(query_result->union_size() +
                                       stats.eval_index_entry_count);
        }
      } else if (it == last_word_it) {
        // perform last intersection and copy
        // matches directly into final result vector
        const stats_type stats(strategy_.intersect_result_set(
            *src_set_ptr, *it, query_result->query(), cur_max_phrase_frequency,
            request.max_phrase_count(), std::back_inserter(index_entries)));
        if (!stats.unknown_word.empty()) {
          query_result->add_unknown_word(stats.unknown_word);
        }
        query_result->set_union_size(query_result->union_size() +
                                     stats.eval_index_entry_count);
      } else {
        // perform intermediate intersection
        const stats_type stats(strategy_.intersect_result_set(
            *src_set_ptr, *it, query_result->query(), cur_max_phrase_frequency,
            std::numeric_limits<size_t>::max(),
            std::inserter(*dst_set_ptr, dst_set_ptr->end())));
        cur_max_phrase_frequency = stats.max_phrase_frequency;
        if (!stats.unknown_word.empty()) {
          query_result->add_unknown_word(stats.unknown_word);
        }
        query_result->set_union_size(query_result->union_size() +
                                     stats.eval_index_entry_count);
        std::swap(src_set_ptr, dst_set_ptr);
        dst_set_ptr->clear();
      }
      if (src_set_ptr->empty())
        break;
    }
    for (auto it = index_entries.begin(); it != index_entries.end(); ++it) {
      generated::QueryResult::PhraseRef* reference =
          query_result->add_reference();
      reference->set_frequency(traits::get_phrase_frequency(*it));
      reference->set_id(traits::get_phrase_id(*it));
      reference->set_length(query.unit_size());
    }
    return query_result;
  }

private:
  retrieval_strategy strategy_;
};

} // namespace netspeak

#endif // NETSPEAK_QUERY_PROCESSOR_HPP
