#ifndef NETSPEAK_RETRIEVAL_STRATEGY_1_HPP
#define NETSPEAK_RETRIEVAL_STRATEGY_1_HPP

#include <cmath>
#include <limits>
#include <memory>
#include <string>

#include "aitools/invertedindex/Configuration.hpp"
#include "aitools/invertedindex/Searcher.hpp"
#include "aitools/util/check.hpp"
#include "aitools/util/logging.hpp"
#include "aitools/value/pair.hpp"
#include "aitools/value/quadruple.hpp"

#include "netspeak/Properties.hpp"
#include "netspeak/RetrievalStrategy.hpp"

namespace netspeak {

struct RetrievalStrategy1Tag {
  /**
   * The entry type of the phrase index, which encodes the following values:
   * entry = ( n-gram-frequency, n-gram-id, n-gram-length, word-position )
   */
  typedef aitools::quadruple<uint32_t, uint32_t, uint8_t, uint8_t>
      index_entry_type;
};

/**
 * Specialized traits template for \c RetrievalStrategy1Tag.
 */
template <> struct index_entry_traits<RetrievalStrategy1Tag> {
  typedef RetrievalStrategy1Tag::index_entry_type value_type;
  typedef value_type::e1_type phrase_frequency_type;
  typedef value_type::e2_type phrase_id_type;
  typedef value_type::e3_type phrase_length_type;
  typedef value_type::e4_type word_position_type;

  static bool equal(const value_type& a, const value_type& b) {
    return hash(a) == hash(b);
  }

  static phrase_frequency_type get_phrase_frequency(const value_type& value) {
    return value.e1();
  }

  static phrase_id_type get_phrase_id(const value_type& value) {
    return value.e2();
  }

  static phrase_length_type get_phrase_length(const value_type& value) {
    return value.e3();
  }

  static word_position_type get_word_position(const value_type& value) {
    return value.e4();
  }

  static size_t hash(const value_type& value) {
    return get_phrase_id(value);
  }

  static void set_phrase_frequency(value_type& value,
                                   phrase_frequency_type frequency) {
    value.set_e1(frequency);
  }

  static void set_phrase_id(value_type& value, phrase_id_type id) {
    value.set_e2(id);
  }

  static void set_phrase_length(value_type& value, phrase_length_type length) {
    value.set_e3(length);
  }

  static void set_word_position(value_type& value,
                                word_position_type position) {
    value.set_e4(position);
  }
};

/**
 * Implements the 1st strategy to lookup and intersect postlists.
 *
 * Special to this version:
 * - There is only one phrase index for all (2-5)-grams.
 * - The key of the phrase index encodes only a word, i.e. "hello" returns a
 *   postlist with references to all phrases having "hello" at any position.
 * - Postlists are read from the beginning to a certain offset, that will be
 *   derived from some quantile value in the range (0, 1]. For that reason
 *   the postlists are indexed as well (postlist_index_).
 * - (jumpin-frequencies are not supported by this version)
 */
template <> class RetrievalStrategy<RetrievalStrategy1Tag> {
public:
  typedef RetrievalStrategy1Tag::index_entry_type index_entry_type;
  typedef index_entry_traits<RetrievalStrategy1Tag> traits;

private:
  struct is_valid_functor {
    bool operator()(const generated::Query::Unit& u, const generated::Query& q,
                    const index_entry_type& e) {
      return traits::get_phrase_length(e) == q.unit_size() &&
             traits::get_word_position(e) == u.position();
    }
  };

public:
  inline void initialize(const Configurations::Map& config) {
    const std::string message = "incomplete configuration";
    aitools::check(Configurations::contains(
                       config, Configurations::path_to_postlist_index),
                   message, Configurations::path_to_postlist_index);
    aitools::check(
        Configurations::contains(config, Configurations::path_to_phrase_index),
        message, Configurations::path_to_phrase_index);

    // Open postlist index.
    aitools::invertedindex::Configuration index_config;
    index_config.set_max_memory_usage(aitools::memory_type::mb1024);
    index_config.set_index_directory(
        config.find(Configurations::path_to_postlist_index)->second);
    postlist_index_.open(index_config);

    // Open ngram index.
    index_config.set_index_directory(
        config.find(Configurations::path_to_phrase_index)->second);
    phrase_index_.open(index_config);
  }

  inline void initialize_query(const generated::Request& request,
                               generated::Query& query) {
    for (int i = 0; i != query.unit_size(); ++i) {
      switch (query.unit(i).tag()) {
        case generated::Query::Unit::WORD:
        case generated::Query::Unit::WORD_IN_DICTSET:
        case generated::Query::Unit::WORD_IN_OPTIONSET:
        case generated::Query::Unit::WORD_IN_ORDERSET:
          query.mutable_unit(i)->set_position(i);
          // We have no frequency value here to decide if that unit is a
          // stopword or not, so all units get the same quantile assigned.
          query.mutable_unit(i)->set_quantile(request.quantile_low());
          break;
        default:;
      }
    }
  }

  template <typename OutputIterator>
  const stats_type initialize_result_set(const generated::Query::Unit& unit,
                                         const generated::Query& query,
                                         size_t max_phrase_frequency,
                                         size_t max_phrase_count,
                                         OutputIterator output) {
    stats_type stats;
    std::shared_ptr<aitools::invertedindex::Postlist<index_entry_type> >
        postlist(search_(unit.text(), unit.quantile()));
    if (!postlist) {
      stats.unknown_word = unit.text();
      return stats;
    }

    // The first of the following two _while_ loops has two tasks:
    // (1) Skip values of the postlist with invalid frequency
    // (2) Select the frequency of the first valid _index_entry_

    is_valid_functor is_valid;
    index_entry_type index_entry;
    while (postlist->next(index_entry) && max_phrase_count != 0) {
      ++stats.eval_index_entry_count;
      if (traits::get_phrase_frequency(index_entry) <= max_phrase_frequency &&
          is_valid(unit, query, index_entry)) {
        --max_phrase_count;
        stats.max_phrase_frequency = traits::get_phrase_frequency(index_entry);
        *output = index_entry;
        ++output;
        break;
      }
    }
    // Copy all remaining index entries.
    while (postlist->next(index_entry) && max_phrase_count != 0) {
      ++stats.eval_index_entry_count;
      if (is_valid(unit, query, index_entry)) {
        --max_phrase_count;
        *output = index_entry;
        ++output;
      }
    }
    stats.min_phrase_frequency = traits::get_phrase_frequency(index_entry);
    return stats;
  }

  template <typename IntersectionSet, typename OutputIterator>
  const stats_type intersect_result_set(const IntersectionSet& input,
                                        const generated::Query::Unit& unit,
                                        const generated::Query& query,
                                        size_t max_phrase_frequency,
                                        size_t max_phrase_count,
                                        OutputIterator output) {
    stats_type stats;
    std::shared_ptr<aitools::invertedindex::Postlist<index_entry_type> >
        postlist(search_(unit.text(), unit.quantile()));
    if (!postlist) {
      stats.unknown_word = unit.text();
      return stats;
    }

    // The first of the following two _while_ loops has two reasons:
    // (1) Skip values of the postlist with invalid frequency
    // (2) Select the frequency of the first valid _index_entry_

    is_valid_functor is_valid;
    index_entry_type index_entry;
    while (postlist->next(index_entry) && max_phrase_count != 0) {
      ++stats.eval_index_entry_count;
      if (traits::get_phrase_frequency(index_entry) <= max_phrase_frequency &&
          is_valid(unit, query, index_entry) &&
          input.find(index_entry) != input.end()) {
        --max_phrase_count;
        stats.max_phrase_frequency = traits::get_phrase_frequency(index_entry);
        *output = index_entry;
        ++output;
        break;
      }
    }
    // Copy all remaining index entries.
    while (postlist->next(index_entry) && max_phrase_count != 0) {
      ++stats.eval_index_entry_count;
      if (is_valid(unit, query, index_entry) &&
          input.find(index_entry) != input.end()) {
        --max_phrase_count;
        *output = index_entry;
        ++output;
      }
    }
    stats.min_phrase_frequency = traits::get_phrase_frequency(index_entry);
    return stats;
  }

  inline Properties::Map properties() const {
    // Should insert phrase_index and postlist_index
    // properties as defined in NetspeakProperties.hpp.
    Properties::Map properties;
    properties[Properties::phrase_index_value_type] =
        phrase_index_.properties().value_type;
    properties[Properties::phrase_index_key_count] =
        aitools::to_string(phrase_index_.properties().key_count);
    properties[Properties::phrase_index_value_sorting] =
        aitools::to_string(phrase_index_.properties().value_sorting);
    properties[Properties::phrase_index_value_count] =
        aitools::to_string(phrase_index_.properties().value_count);
    properties[Properties::phrase_index_total_size] =
        aitools::to_string(phrase_index_.properties().total_size);

    properties[Properties::postlist_index_value_type] =
        postlist_index_.properties().value_type;
    properties[Properties::postlist_index_key_count] =
        aitools::to_string(postlist_index_.properties().key_count);
    properties[Properties::postlist_index_value_sorting] =
        aitools::to_string(postlist_index_.properties().value_sorting);
    properties[Properties::postlist_index_value_count] =
        aitools::to_string(postlist_index_.properties().value_count);
    properties[Properties::postlist_index_total_size] =
        aitools::to_string(postlist_index_.properties().total_size);
    return properties;
  }

private:
  inline std::shared_ptr<aitools::invertedindex::Postlist<index_entry_type> >
  search_(const std::string& key, double quantile) {
    // This function maps a quantile value in the range (0, 1] to some
    // valid index value in the range [0, resolution-1]. If the quantile is
    // out of range an exception will be thrown. Do NOT change the value
    // type of the quantile from double to float, b/c of rounding errors.
    struct quantile_to_index {
      size_t operator()(double quantile, size_t resolution) {
        // Do never change this w/o unit testing.
        const size_t index(
            std::floor(static_cast<float>(quantile * resolution + 0.9)) - 1);
        if (index >= resolution) {
          aitools::throw_out_of_range(__func__, index);
        }
        DEBUG_LOG2(__func__, index);
        return index;
      }
    } mapper;

    size_t idx_end = std::numeric_limits<uint32_t>::max();
    std::unique_ptr<
        aitools::invertedindex::Postlist<postlist_index_value_type> >
        meta_postlist(postlist_index_.search_postlist(key));
    if (meta_postlist) { // Get index_end from quantile if meta index exist.
      const size_t index_tmp = mapper(quantile, meta_postlist->size());
      postlist_index_value_type value;
      for (unsigned i = 0; i <= index_tmp; ++i) {
        meta_postlist->next(value);
      }
      idx_end = value.e1() + 1;
    }
    return std::shared_ptr<aitools::invertedindex::Postlist<index_entry_type> >(
        phrase_index_.search_postlist(key, 0, idx_end));
  }

  typedef aitools::pair<uint32_t, uint32_t> postlist_index_value_type;

  aitools::invertedindex::Searcher<postlist_index_value_type> postlist_index_;
  aitools::invertedindex::Searcher<index_entry_type> phrase_index_;
};

} // namespace netspeak

#endif // NETSPEAK_RETRIEVAL_STRATEGY_1_HPP
