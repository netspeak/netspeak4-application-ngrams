#ifndef NETSPEAK_NETSPEAK_HPP
#define NETSPEAK_NETSPEAK_HPP

#include <netspeak/NetspeakService.pb.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>
#include <typeinfo>

#include "../antlr4/parse.hpp"
#include "Configuration.hpp"
#include "Dictionaries.hpp"
#include "PhraseCorpus.hpp"
#include "PhraseDictionary.hpp"
#include "Properties.hpp"
#include "QueryNormalizer.hpp"
#include "QueryProcessor.hpp"
#include "RetrievalStrategy3.hpp"
#include "model/NormQuery.hpp"
#include "model/Phrase.hpp"
#include "model/Query.hpp"
#include "model/RawPhraseResult.hpp"
#include "model/RawRefResult.hpp"
#include "model/RawResult.hpp"
#include "model/SearchOptions.hpp"
#include "model/SearchResult.hpp"
#include "regex/DefaultRegexIndex.hpp"
#include "util/LfuCache.hpp"
#include "util/check.hpp"
#include "util/logging.hpp"
#include "value/string_traits.hpp"
#include "value/value_traits.hpp"


namespace netspeak {

class Netspeak final {
public:
  Netspeak() = default;
  Netspeak(const Netspeak&) = delete;

  void initialize(const Configuration& config);

  Properties properties() const;

  void search(const service::SearchRequest& request, service::SearchResponse& response) noexcept;


private:
  using NormQuery = model::NormQuery;
  using Query = model::Query;
  using RawPhraseResult = model::RawPhraseResult;
  using RawRefResult = model::RawRefResult;
  using RawResult = model::RawResult;
  using SearchOptions = model::SearchOptions;

  std::pair<QueryNormalizer::Options, SearchOptions> to_options(const service::SearchRequest& request);

  std::unique_ptr<SearchResult> merge_raw_result_(const SearchOptions& options, const RawResult& raw_result);

  std::shared_ptr<const RawRefResult> process_wildcard_query_(const SearchOptions& options, const NormQuery& query);
  std::shared_ptr<const RawPhraseResult> process_non_wildcard_query_(const SearchOptions& options,
                                                                     const NormQuery& query);

  std::unique_ptr<RawResult> search_raw_(const QueryNormalizer::Options& normalizer_options,
                                         const SearchOptions& options, std::shared_ptr<Query> query);

  struct search_config {
    size_t max_norm_queries;
    size_t regex_max_matches;
    std::chrono::nanoseconds regex_max_time;
  };
  search_config get_search_config(const Configuration& config) const;

private:
  /** \todo Extract the whole cache logic into its own class **/
  struct result_cache_item {
  public:
    SearchOptions options;
    std::shared_ptr<const RawRefResult> result;

    result_cache_item() = delete;
    result_cache_item(const SearchOptions& options, const std::shared_ptr<const RawRefResult>& result)
        : options(options), result(result) {}
  };

  std::shared_ptr<Dictionaries::Map> hash_dictionary_;
  std::shared_ptr<regex::DefaultRegexIndex> regex_index_;
  std::unique_ptr<PhraseDictionary> phrase_dictionary_;
  QueryNormalizer query_normalizer_;
  QueryProcessor<RetrievalStrategy3Tag> query_processor_;
  util::LfuCache<result_cache_item> result_cache_;
  PhraseCorpus phrase_corpus_;
  search_config search_config_;
};

} // namespace netspeak

#endif // NETSPEAK_NETSPEAK_HPP
