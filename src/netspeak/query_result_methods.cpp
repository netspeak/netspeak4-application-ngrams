#include "netspeak/query_result_methods.hpp"

#include <algorithm>
#include <unordered_set>

#include "netspeak/request_methods.hpp"

namespace netspeak {

bool do_intersect(const generated::QueryResult& a,
                  const generated::QueryResult& b) {
  return is_compatible(a.request(), b.request()) &&
         do_intersect(a.reference().begin(), a.reference().end(),
                      b.reference().begin(), b.reference().end());
}

std::shared_ptr<generated::QueryResult> merge(
    std::shared_ptr<generated::QueryResult> a,
    std::shared_ptr<generated::QueryResult> b) {
  std::shared_ptr<generated::QueryResult> result(new generated::QueryResult);
  result->mutable_request()->CopyFrom(a->request());
  result->mutable_query()->CopyFrom(a->query());
  merge(a->reference().begin(), a->reference().end(), b->reference().begin(),
        b->reference().end(), result->mutable_reference());
  // update the new QueryResult::request
  result->mutable_request()->set_max_phrase_count(result->reference_size());
  result->mutable_request()->set_max_phrase_frequency(
      std::max(a->request().max_phrase_frequency(),
               b->request().max_phrase_frequency()));
  return result;
}

std::shared_ptr<generated::QueryResult> prune(
    std::shared_ptr<generated::QueryResult> result,
    const generated::Request& request) {
  std::shared_ptr<generated::QueryResult> pruned_result(
      new generated::QueryResult);
  pruned_result->mutable_query()->CopyFrom(result->query());
  pruned_result->mutable_request()->CopyFrom(request);
  // Copy QueryResult::reference
  for (const auto& phrase_ref : result->reference()) {
    if (phrase_ref.frequency() <= request.max_phrase_frequency() &&
        phrase_ref.length() >= request.phrase_length_min() &&
        phrase_ref.length() <= request.phrase_length_max()) {
      pruned_result->add_reference()->CopyFrom(phrase_ref);
    }
  }
  // Copy QueryResult::phrase
  const uint32_t num_words = result->phrase().word_size();
  if (result->phrase().frequency() <= request.max_phrase_frequency() &&
      num_words >= request.phrase_length_min() &&
      num_words <= request.phrase_length_max()) {
    pruned_result->mutable_phrase()->CopyFrom(result->phrase());
  }
  // Copy QueryResult::unknown_word
  pruned_result->mutable_unknown_word()->MergeFrom(result->unknown_word());
  return pruned_result;
}

namespace generated {

bool operator==(const QueryResult::PhraseRef& a,
                const QueryResult::PhraseRef& b) {
  return a.length() == b.length() && a.id() == b.id();
}

bool operator<(const QueryResult::PhraseRef& a,
               const QueryResult::PhraseRef& b) {
  return a.frequency() < b.frequency();
}

bool operator>(const QueryResult::PhraseRef& a,
               const QueryResult::PhraseRef& b) {
  return a.frequency() > b.frequency();
}

} // namespace generated
} // namespace netspeak
