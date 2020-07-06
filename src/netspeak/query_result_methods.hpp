#ifndef NETSPEAK_QUERY_RESULT_METHODS_HPP
#define NETSPEAK_QUERY_RESULT_METHODS_HPP

#include <algorithm>
#include <memory>

//#include "netspeak/generated/NetspeakMessages.pb.h"

namespace netspeak {

/**
 * Checks if two PhraseRef sequences overlap with respect to their frequencies.
 * Precondition:
 * - Both ranges are sorted in descending order.
 */
template <typename PhraseRefInputIterator>
bool do_intersect(PhraseRefInputIterator a_first, PhraseRefInputIterator a_last,
                  PhraseRefInputIterator b_first,
                  PhraseRefInputIterator b_last) {
  if (a_first == a_last || b_first == b_last)
    return true;
  const auto a_min_freq = std::prev(a_last)->frequency();
  const auto a_max_freq = a_first->frequency();
  const auto b_min_freq = std::prev(b_last)->frequency();
  const auto b_max_freq = b_first->frequency();
  return a_max_freq >= b_max_freq ? a_min_freq <= b_max_freq
                                  : b_min_freq <= a_max_freq;
}

/**
 * Checks if the lists of references of two query results overlap.
 * Precondition:
 * - QueryResult::request::query are equal.
 * - QueryResult::reference are sorted in descending order.
 */
bool do_intersect(const generated::QueryResult& a,
                  const generated::QueryResult& b);

/**
 * Merges two ranges of references to build a union set.
 * Precondition:
 * - Both ranges are sorted in descending order.
 */
template <typename PhraseRefInputIterator>
google::protobuf::RepeatedPtrField<generated::QueryResult::PhraseRef>* merge(
    PhraseRefInputIterator a_first, PhraseRefInputIterator a_last,
    PhraseRefInputIterator b_first, PhraseRefInputIterator b_last,
    google::protobuf::RepeatedPtrField<generated::QueryResult::PhraseRef>*
        result) {
  std::vector<generated::QueryResult::PhraseRef> union_set;
  std::set_union(a_first, a_last, b_first, b_last,
                 std::back_inserter(union_set),
                 std::greater<generated::QueryResult::PhraseRef>());
  copy(union_set.begin(), union_set.end(), result);
  return result;
}

/**
 * Merges two query results unless if their reference lists intersect or not.
 * Preconditions:
 * - do_intersect(a, b) is true.
 */
std::shared_ptr<generated::QueryResult> merge(
    std::shared_ptr<generated::QueryResult> a,
    std::shared_ptr<generated::QueryResult> b);

std::shared_ptr<generated::QueryResult> prune(
    std::shared_ptr<generated::QueryResult> result,
    const generated::Request& request);

// -----------------------------------------------------------------------------
// Operator support
// -----------------------------------------------------------------------------

namespace generated {
// Operators must be defined in the same namespace as its operands for
// argument-dependent lookup (ADL, also known as Koenig lookup).

bool operator==(const QueryResult::PhraseRef& a,
                const QueryResult::PhraseRef& b);

bool operator<(const QueryResult::PhraseRef& a,
               const QueryResult::PhraseRef& b);

bool operator>(const QueryResult::PhraseRef& a,
               const QueryResult::PhraseRef& b);

} // namespace generated
} // namespace netspeak

#endif // NETSPEAK_QUERY_RESULT_METHODS_HPP
