#ifndef NETSPEAK_QUERY_NORMALIZER_HPP
#define NETSPEAK_QUERY_NORMALIZER_HPP

#include <memory>
#include <string>
#include <vector>

#include "netspeak/Dictionaries.hpp"
#include "netspeak/internal/NormQuery.hpp"
#include "netspeak/internal/Query.hpp"
#include "netspeak/regex/RegexIndex.hpp"


namespace netspeak {

using namespace internal;
using namespace regex;

// TODO: cache for regex queries


/**
 * This class is responsable for normalizing queries.
 *
 * A norm query (normalized query)
 */
class QueryNormalizer {
private:
  std::shared_ptr<RegexIndex> regex_index_;
  std::shared_ptr<Dictionaries::Map> dictionary_;

public:
  typedef struct InitConfig {
    std::shared_ptr<RegexIndex> regex_index;
    std::shared_ptr<Dictionaries::Map> dictionary;
  } InitConfig;

  QueryNormalizer(const InitConfig& config);
  QueryNormalizer() = delete;
  QueryNormalizer(const QueryNormalizer&) = delete;

  typedef struct Options {
    /**
     * @brief The maximum number of norm queries allowed to be returned.
     */
    size_t max_norm_queries;

    /**
     * @brief The maximum length norm queries are allowed to have.
     *
     * This means that only phrases up to this length can be matched by the norm
     * queries.
     */
    size_t max_length;
    /**
     * @brief Same as \c max_length but as a minimum.
     */
    size_t min_length;

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

    Options(const Options&) = default;
  } Options;

  void normalize(const Query& query, const Options& options,
                 std::vector<NormQuery>& norm_queries);
};

} // namespace netspeak


#endif
