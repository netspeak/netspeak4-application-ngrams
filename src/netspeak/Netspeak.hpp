#ifndef NETSPEAK_NETSPEAK_HPP
#define NETSPEAK_NETSPEAK_HPP

#include <algorithm>
#include <memory>
#include <string>
#include <typeinfo>

#include <boost/filesystem.hpp>

#include "aitools/util/check.hpp"
#include "aitools/util/logging.hpp"
#include "aitools/value/string_traits.hpp"
#include "aitools/value/value_traits.hpp"

#include "antlr4/parse.hpp"

#include "netspeak/Configurations.hpp"
#include "netspeak/Dictionaries.hpp"
#include "netspeak/PhraseCorpus.hpp"
#include "netspeak/PhraseDictionary.hpp"
#include "netspeak/Properties.hpp"
#include "netspeak/QueryNormalizer.hpp"
#include "netspeak/QueryProcessor.hpp"
#include "netspeak/RetrievalStrategy3.hpp"
#include "netspeak/internal/NormQuery.hpp"
#include "netspeak/internal/Phrase.hpp"
#include "netspeak/internal/Query.hpp"
#include "netspeak/internal/RawPhraseResult.hpp"
#include "netspeak/internal/RawRefResult.hpp"
#include "netspeak/internal/RawResult.hpp"
#include "netspeak/internal/SearchOptions.hpp"
#include "netspeak/internal/SearchResult.hpp"
#include "netspeak/regex/DefaultRegexIndex.hpp"
#include "netspeak/service/NetspeakService.pb.h"
#include "netspeak/util/LfuCache.hpp"


namespace netspeak {

class Netspeak {
public:
  Netspeak() {}
  Netspeak(const Netspeak&) = delete;

  void initialize(const Configurations::Map& config);

  const Properties::Map properties() const;

  void search(const service::SearchRequest& request,
              service::SearchResponse& response) throw();


private:
  typedef internal::Query Query;
  typedef internal::NormQuery NormQuery;
  typedef internal::RawPhraseResult RawPhraseResult;
  typedef internal::RawRefResult RawRefResult;
  typedef internal::RawResult RawResult;
  typedef internal::SearchOptions SearchOptions;

  std::pair<QueryNormalizer::Options, SearchOptions> to_options(
      const service::SearchRequest& request);

  std::unique_ptr<SearchResult> merge_raw_result_(const SearchOptions& options,
                                                  const RawResult& raw_result);

  std::shared_ptr<const RawRefResult> process_wildcard_query_(
      const SearchOptions& options, const NormQuery& query);
  std::shared_ptr<const RawPhraseResult> process_non_wildcard_query_(
      const SearchOptions& options, const NormQuery& query);

  std::unique_ptr<RawResult> search_raw_(
      const QueryNormalizer::Options& normalizer_options,
      const SearchOptions& options, std::shared_ptr<Query> query);

private:
  // TODO: Extract the whole cache logic into its own class
  struct result_cache_item {
  public:
    SearchOptions options;
    std::shared_ptr<const RawRefResult> result;

    result_cache_item() = delete;
    result_cache_item(const SearchOptions& options,
                      const std::shared_ptr<const RawRefResult>& result)
        : options(options), result(result) {}
  };

  std::shared_ptr<Dictionaries::Map> hash_dictionary_;
  std::shared_ptr<regex::DefaultRegexIndex> regex_index_;
  std::unique_ptr<PhraseDictionary> phrase_dictionary_;
  QueryNormalizer query_normalizer_;
  QueryProcessor<RetrievalStrategy3Tag> query_processor_;
  util::LfuCache<result_cache_item> result_cache_;
  PhraseCorpus phrase_corpus_;
};

} // namespace netspeak

#endif // NETSPEAK_NETSPEAK_HPP
