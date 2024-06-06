#ifndef NETSPEAK_QUERY_NORMALIZER_HPP
#define NETSPEAK_QUERY_NORMALIZER_HPP

#include <memory>
#include <string>
#include <vector>

#include "Dictionaries.hpp"
#include "model/NormQuery.hpp"
#include "model/Query.hpp"
#include "regex/RegexIndex.hpp"


namespace netspeak {

/** \todo cache for regex queries **/


/**
 * This class is responsible for normalizing queries.
 *
 * A norm query (normalized query)
 */
class QueryNormalizer {
private:
  std::shared_ptr<regex::RegexIndex> regex_index_;
  std::shared_ptr<Dictionaries::Map> dictionary_;
  bool lower_case;

public:
  struct InitConfig {
    std::shared_ptr<regex::RegexIndex> regex_index = nullptr;
    std::shared_ptr<Dictionaries::Map> dictionary = nullptr;
    bool lower_case = false;
  };

  QueryNormalizer() {}
  explicit QueryNormalizer(InitConfig config);
  QueryNormalizer(const QueryNormalizer&) = delete;

  struct Options {
    /**
     * @brief The maximum number of norm queries allowed to be returned.
     */
    size_t max_norm_queries;

    /**
     * @brief Same as \c max_length but as a minimum.
     */
    uint32_t min_length;
    /**
     * @brief The maximum length norm queries are allowed to have.
     *
     * This means that only phrases up to this length can be matched by the norm
     * queries.
     */
    uint32_t max_length;

    /**
     * @brief The maximum number of words each regex can be replaced with.
     *
     * A regex will (usually) be replaced with an alternation of words that
     * match that regex. This value determines with how many words each regex
     * will be replaced with at most.
     */
    size_t max_regex_matches;
    /**
     * @brief The maximum amount of time spend on each regex to find matching
     * words for that regex.
     */
    std::chrono::nanoseconds max_regex_time;
  };

  void normalize(std::shared_ptr<const model::Query> query, const Options& options,
                 std::vector<model::NormQuery>& norm_queries);
};

} // namespace netspeak


#endif
