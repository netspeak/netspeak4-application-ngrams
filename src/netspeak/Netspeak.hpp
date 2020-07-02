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

#include "netspeak/Configurations.hpp"
#include "netspeak/Dictionaries.hpp"
#include "netspeak/util/LfuCache.hpp"
#include "netspeak/PhraseCorpus.hpp"
#include "netspeak/PhraseDictionary.hpp"
#include "netspeak/Properties.hpp"
#include "netspeak/QueryProcessor.hpp"
#include "netspeak/error.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/postprocessing.hpp"
#include "netspeak/protobuf_utils.hpp"
#include "netspeak/query_normalization.hpp"
#include "netspeak/query_result_methods.hpp"
#include "netspeak/regex/DefaultRegexIndex.hpp"
#include "netspeak/request_methods.hpp"


namespace netspeak {

const size_t MAX_QUERY_LENGTH = 200;

namespace bfs = boost::filesystem;

template <typename RetrievalStrategyTag> class Netspeak {
public:
  Netspeak(const Netspeak&) = delete;

  void initialize(const Configurations::Map& config) {
    Configurations::Map conf(config);
    const auto it = conf.find(Configurations::path_to_home);
    if (it != conf.end()) {
      conf[Configurations::path_to_phrase_corpus] =
          it->second + "/" + Configurations::default_phrase_corpus_dir_name;
      conf[Configurations::path_to_phrase_dictionary] =
          it->second + "/" + Configurations::default_phrase_dictionary_dir_name;
      conf[Configurations::path_to_phrase_index] =
          it->second + "/" + Configurations::default_phrase_index_dir_name;
      conf[Configurations::path_to_postlist_index] =
          it->second + "/" + Configurations::default_postlist_index_dir_name;
      conf[Configurations::path_to_hash_dictionary] =
          it->second + "/" + Configurations::default_hash_dictionary_dir_name;
      conf[Configurations::path_to_regex_vocabulary] =
          it->second + "/" + Configurations::default_regex_vocabulary_dir_name;
    }
    const std::string msg = "incomplete configuration";
    const auto pcd = conf.find(Configurations::path_to_phrase_corpus);
    aitools::check(pcd != conf.end(), msg,
                   Configurations::path_to_phrase_corpus);
    const auto pdd = conf.find(Configurations::path_to_phrase_dictionary);
    aitools::check(pdd != conf.end(), msg,
                   Configurations::path_to_phrase_dictionary);
    const auto cc = conf.find(Configurations::cache_capacity);
    aitools::check(cc != conf.end(), msg, Configurations::cache_capacity);
    result_cache_.reserve(std::stoul(cc->second));

    auto dir = bfs::path(pdd->second);
    aitools::log("Open phrase dictionary in", dir);
    phrase_dictionary_.reset(
        PhraseDictionary::Open(dir, aitools::memory_type::min_required));

    dir = bfs::path(pcd->second) / "bin";
    aitools::log("Open phrase corpus in", dir);
    phrase_corpus_.open(dir);

    // The hash dictionary is optional.
    auto sdd = conf.find(Configurations::path_to_hash_dictionary);
    if (sdd != conf.end() && bfs::exists(sdd->second)) {
      const bfs::directory_iterator end;
      for (bfs::directory_iterator it(sdd->second); it != end; ++it) {
        aitools::log("Open hash dictionary in", *it);
        const auto dict = Dictionaries::read_from_file(*it);
        //        hash_dictionary_.insert(dict.cbegin(), dict.cend());  //
        // ambiguous?
        for (const auto& pair : dict) {
          hash_dictionary_.insert(pair);
        }
      }
    }

    // The regex vocabulary is optional.
    sdd = conf.find(Configurations::path_to_regex_vocabulary);
    if (sdd != conf.end() && bfs::exists(sdd->second)) {
      const bfs::directory_iterator end;
      for (bfs::directory_iterator it(sdd->second); it != end; ++it) {
        aitools::log("Open regex vocabulary in", *it);
        bfs::ifstream ifs(*it);
        aitools::check(ifs.is_open(), error_message::cannot_open, *it);
        std::string regexwords((std::istreambuf_iterator<char>(ifs)),
                               (std::istreambuf_iterator<char>()));
        ifs.close();

        regex_index_vocabulary = regexwords;
        regex_index_ = std::shared_ptr<netspeak::regex::DefaultRegexIndex>(
            new netspeak::regex::DefaultRegexIndex(regex_index_vocabulary));
        break;
      }
    }
    query_processor_.initialize(conf);
  }

  const Properties::Map properties() const {
    // Should return phrase_index and postlist_index
    // properties as defined in Properties.hpp.
    auto properties = query_processor_.properties();
    // cache properties
    properties[Properties::cache_policy] = "least frequently used";
    properties[Properties::cache_size] = std::to_string(result_cache_.size());
    properties[Properties::cache_capacity] =
        std::to_string(result_cache_.capacity());
    properties[Properties::cache_access_count] =
        std::to_string(result_cache_.access_count());
    properties[Properties::cache_hit_rate] =
        std::to_string(result_cache_.hit_rate());
    std::ostringstream oss;
    result_cache_.list(oss, 100);
    properties[Properties::cache_top_100] = oss.str();
    // phrase corpus properties
    properties[Properties::phrase_corpus_1gram_count] =
        std::to_string(phrase_corpus_.count_phrases(1));
    properties[Properties::phrase_corpus_2gram_count] =
        std::to_string(phrase_corpus_.count_phrases(2));
    properties[Properties::phrase_corpus_3gram_count] =
        std::to_string(phrase_corpus_.count_phrases(3));
    properties[Properties::phrase_corpus_4gram_count] =
        std::to_string(phrase_corpus_.count_phrases(4));
    properties[Properties::phrase_corpus_5gram_count] =
        std::to_string(phrase_corpus_.count_phrases(5));
    // phrase dictionary properties
    properties[Properties::phrase_dictionary_size] =
        std::to_string(phrase_dictionary_->size());
    properties[Properties::phrase_dictionary_value_type] =
        aitools::value::value_traits<PhraseDictionary::Value>::type_name();
    // hash dictionary properties
    properties[Properties::hash_dictionary_size] =
        std::to_string(hash_dictionary_.size());
    properties[Properties::hash_dictionary_value_type] =
        aitools::value::value_traits<
            Dictionaries::Map::mapped_type>::type_name();
    // regex vocabulary properties
    // BEWARE: The index is optional!
    auto vocal_size = regex_index_ ? regex_index_vocabulary.size() : 0;
    properties[Properties::regex_vocabulary_size] = std::to_string(vocal_size);
    properties[Properties::regex_vocabulary_value_type] =
        std::string(typeid(netspeak::regex::DefaultRegexIndex).name());
    return properties;
  }

  std::shared_ptr<generated::Response> search(
      const generated::Request& request) throw() {
    std::shared_ptr<generated::Response> response;
    try {
      size_t length = request.query().size();
      if (length > MAX_QUERY_LENGTH)
        throw std::logic_error(netspeak::query_error_message::too_long_query(
            length, MAX_QUERY_LENGTH));
      response = merge_raw_response_(search_raw(request));
    } catch (std::logic_error error) {
      response.reset(new generated::Response);
      response->mutable_request()->CopyFrom(request);
      response->set_error_code(to_ordinal(error_code::size));
      response->set_error_message(error.what());
    } catch (...) {
      response.reset(new generated::Response);
      response->mutable_request()->CopyFrom(request);
      response->set_error_code(to_ordinal(error_code::unknown_error));
    }
    return response;
  }

  std::shared_ptr<generated::RawResponse> search_raw(
      const generated::Request& request) throw() {
    generated::Request initialized_request = init_request(request);
    std::shared_ptr<generated::RawResponse> response;
    try {
      response = search_raw_(initialized_request);
    } catch (...) {
      response.reset(new generated::RawResponse);
      response->mutable_request()->CopyFrom(initialized_request);
      response->set_error_code(to_ordinal(error_code::unknown_error));
    }
    return response;
  }

private:
  static generated::Request init_request(const generated::Request& request) {
    generated::Request initialized_request(request);
    if (initialized_request.max_phrase_frequency() == 0) {
      initialized_request.set_max_phrase_frequency(
          std::numeric_limits<uint64_t>::max());
    }

    initialized_request.set_quantile_high(
        std::min(initialized_request.quantile_high(), 1.0));
    initialized_request.set_quantile_high(
        std::max(initialized_request.quantile_high(), 0.0));
    initialized_request.set_quantile_low(
        std::min(initialized_request.quantile_low(), 1.0));
    initialized_request.set_quantile_low(
        std::max(initialized_request.quantile_low(), 0.0));

    return initialized_request;
  }

  std::shared_ptr<generated::Response> merge_raw_response_(
      std::shared_ptr<generated::RawResponse> raw_response) {
    const auto unique_phrase_refs =
        merge_phrase_refs(raw_response->query_result());
    // Extract the topk PhraseRefs from the *sorted* set.
    std::vector<generated::QueryResult::PhraseRef> topk_unique_phrase_refs;
    const auto topk = std::min(
        unique_phrase_refs.size(),
        static_cast<size_t>(raw_response->request().max_phrase_count()));
    std::copy_n(unique_phrase_refs.begin(), topk,
                std::back_inserter(topk_unique_phrase_refs));
    // Read all topk Phrases found by wildcard queries.
    std::vector<generated::Phrase> phrases_from_wildcard_queries =
        phrase_corpus_.read_phrases(topk_unique_phrase_refs);
    // Tag all read topk Phrases found by wildcard queries.
    for (unsigned i = 0; i != phrases_from_wildcard_queries.size(); ++i) {
      const auto query_id = topk_unique_phrase_refs.at(i).query_id();
      tag_phrase(phrases_from_wildcard_queries[i],
                 raw_response->query_result(query_id).query());
    }
    // Copy all read topk Phrases into a final *sorted* merge set.
    std::set<generated::Phrase, std::greater<generated::Phrase> >
        final_unique_sorted_phrases(phrases_from_wildcard_queries.begin(),
                                    phrases_from_wildcard_queries.end());
    // Add already tagged phrases found by non-wildcard queries.
    for (const auto& query_result : raw_response->query_result()) {
      if (query_result.phrase().word_size() == 0)
        continue;
      final_unique_sorted_phrases.insert(query_result.phrase());
    }
    // Copy the final topk Phrases into the Response.
    std::shared_ptr<generated::Response> response(new generated::Response());
    copy_n(
        final_unique_sorted_phrases.begin(), final_unique_sorted_phrases.end(),
        raw_response->request().max_phrase_count(), response->mutable_phrase());
    // Merge and copy all unknown words into the Response.
    copy(merge_unknown_words(raw_response->query_result()),
         response->mutable_unknown_word());
    // Copy the rest into Response.
    response->mutable_request()->CopyFrom(raw_response->request());
    response->mutable_query_token()->MergeFrom(raw_response->query_token());
    response->mutable_query()->CopyFrom(raw_response->query());
    response->set_error_code(raw_response->error_code());
    response->set_error_message(raw_response->error_message());
    response->set_resume_frequency(
        compute_resume_frequency(raw_response->query_result()));
    response->set_total_union_size(
        compute_total_union_size(raw_response->query_result()));
    response->set_total_frequency(
        compute_total_frequency(response->query(), response->phrase()));
    return response;
  }

  void process_(const std::shared_ptr<generated::RawResponse> response,
                const std::vector<generated::Query> normQueries) {
    for (const auto& query : normQueries) {
      generated::Request norm_query_request(response->request());
      norm_query_request.set_query(to_string(query));
      response->add_query_result()->CopyFrom(
          has_wildcard(query)
              ? *process_wildcard_query_(norm_query_request, query)
              : *process_non_wildcard_query_(norm_query_request, query));
    }
  }


  std::shared_ptr<generated::QueryResult> process_wildcard_query_(
      const generated::Request& request, const generated::Query& query) {
    const auto query_string = to_string(query);
    std::shared_ptr<generated::QueryResult> final_result,
        cached_result = result_cache_.find(query_string);
    if (cached_result && cached_result->request() >= request) {
      final_result = prune(cached_result, request);
    } else if (cached_result && cached_result->request() == request) {
      final_result = cached_result;
    } else {
      final_result = query_processor_.process(request, query);
      if (cached_result && do_intersect(*cached_result, *final_result)) {
        result_cache_.update(query_string, merge(cached_result, final_result));
      } else {
        result_cache_.insert(query_string, final_result);
      }
    }
    final_result->mutable_query()->CopyFrom(query);
    return final_result;
  }

  std::shared_ptr<generated::QueryResult> process_non_wildcard_query_(
      const generated::Request& request, const generated::Query& query) {
    std::shared_ptr<generated::QueryResult> result(new generated::QueryResult);
    result->mutable_request()->CopyFrom(request);
    result->mutable_query()->CopyFrom(query);
    PhraseDictionary::Value freq_id_pair;
    if (request.max_phrase_count() > 0 &&
        phrase_dictionary_->Get(to_string(query), freq_id_pair) &&
        freq_id_pair.e1() <= request.max_phrase_frequency()) {
      result->mutable_phrase()->set_frequency(freq_id_pair.e1());
      result->mutable_phrase()->set_id(
          make_unique_id(query.unit_size(), freq_id_pair.e2()));
      for (const auto& unit : query.unit()) {
        result->mutable_phrase()->add_word()->set_text(unit.text());
      }
      tag_phrase(*result->mutable_phrase(), query);
    } else { // find unknown words
      for (const auto& unit : query.unit()) {
        if (!phrase_dictionary_->Get(unit.text(), freq_id_pair)) {
          result->add_unknown_word(unit.text());
        }
      }
    }
    return result;
  }

  /**
   * @brief Performs a search with the given initialized request.
   *
   * @param request
   * @return std::shared_ptr<generated::RawResponse>
   */
  std::shared_ptr<generated::RawResponse> search_raw_(
      const generated::Request& request) {
    std::shared_ptr<generated::RawResponse> response(
        new generated::RawResponse());
    try {
      std::vector<netspeak::generated::Query> normQueries;
      response->mutable_request()->CopyFrom(request);
      parse(response, &hash_dictionary_, regex_index_.get(), &normQueries);
      process_(response, normQueries);
    } catch (const invalid_query& error) {
      response.reset(new generated::RawResponse());
      response->mutable_request()->CopyFrom(request);
      response->set_error_code(to_ordinal(error_code::invalid_query));
      response->set_error_message(error.what());
    } catch (const std::exception& error) {
      response.reset(new generated::RawResponse);
      response->mutable_request()->CopyFrom(request);
      response->set_error_code(to_ordinal(error_code::server_error));
      response->set_error_message(error.what());
    }
    return response;
  }

private:
  Dictionaries::Map hash_dictionary_;
  std::string regex_index_vocabulary;
  std::shared_ptr<netspeak::regex::DefaultRegexIndex> regex_index_;
  std::unique_ptr<PhraseDictionary> phrase_dictionary_;
  QueryProcessor<RetrievalStrategyTag> query_processor_;
  LfuCache<generated::QueryResult> result_cache_;
  PhraseCorpus phrase_corpus_;
};

} // namespace netspeak

#endif // NETSPEAK_NETSPEAK_HPP
