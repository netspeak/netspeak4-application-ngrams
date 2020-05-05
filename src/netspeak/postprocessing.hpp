#ifndef NETSPEAK_POSTPROCESSING_HPP
#define NETSPEAK_POSTPROCESSING_HPP

#include <iterator>
#include <list>
#include <set>

#include "aitools/util/check.hpp"

#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/phrase_methods.hpp"
#include "netspeak/query_methods.hpp"

namespace netspeak {

template <typename QueryResultInputIterator>
uint64_t compute_resume_frequency(QueryResultInputIterator first,
                                  QueryResultInputIterator last) {
  uint64_t resume_frequency(0);
  while (first != last) {
    if (first->reference_size() != 0) {
      resume_frequency =
          std::max(resume_frequency,
                   static_cast<uint64_t>(
                       std::prev(first->reference().end())->frequency()));
    }
    ++first;
  }
  return resume_frequency;
}

template <typename QueryResultContainer>
uint64_t compute_resume_frequency(const QueryResultContainer& results) {
  return compute_resume_frequency(results.begin(), results.end());
}

template <typename PhraseInputIterator>
uint64_t compute_total_frequency(const generated::Query& query,
                                 PhraseInputIterator first,
                                 PhraseInputIterator last) {
  uint64_t frequency(0);
  if (starts_with(query, generated::Query::Unit::ASTERISK) ||
      ends_with(query, generated::Query::Unit::ASTERISK)) {
    std::list<generated::Phrase> phrases(first, last);
    for (auto lhs = phrases.begin(); lhs != phrases.end(); ++lhs) {
      for (auto rhs = std::next(lhs); rhs != phrases.end();) {
        rhs = is_subphrase_of(*lhs, *rhs) ? phrases.erase(rhs) : std::next(rhs);
      }
    }
    for (auto it = phrases.begin(); it != phrases.end(); ++it) {
      frequency += it->frequency();
    }
  } else {
    while (first != last) {
      frequency += first->frequency();
      ++first;
    }
  }
  return frequency;
}

template <typename PhraseContainer>
uint64_t compute_total_frequency(const generated::Query& query,
                                 const PhraseContainer& phrases) {
  return compute_total_frequency(query, phrases.begin(), phrases.end());
}

template <typename QueryResultInputIterator>
uint64_t compute_total_union_size(QueryResultInputIterator first,
                                  QueryResultInputIterator last) {
  uint64_t total_union_size(0);
  while (first != last) {
    total_union_size += first->union_size();
    ++first;
  }
  return total_union_size;
}

template <typename QueryResultContainer>
uint64_t compute_total_union_size(const QueryResultContainer& results) {
  return compute_total_union_size(results.begin(), results.end());
}

template <typename QueryResultInputIterator>
std::set<generated::QueryResult::PhraseRef,
         std::greater<generated::QueryResult::PhraseRef> >
merge_phrase_refs(QueryResultInputIterator first,
                  QueryResultInputIterator last) {
  typedef generated::QueryResult::PhraseRef PhraseRef;
  std::set<PhraseRef, std::greater<PhraseRef> > refs;
  for (unsigned i = 0; first != last; ++first, ++i) {
    for (auto it = first->reference().begin(); it != first->reference().end();
         ++it) {
      const auto ret = refs.insert(*it);
      if (ret.second) {
        // Remember the id of the original normalized query.
        const_cast<PhraseRef&>(*ret.first).set_query_id(i);
      }
    }
  }
  return refs;
}

template <typename QueryResultContainer>
std::set<generated::QueryResult::PhraseRef,
         std::greater<generated::QueryResult::PhraseRef> >
merge_phrase_refs(const QueryResultContainer& results) {
  return merge_phrase_refs(results.begin(), results.end());
}

template <typename QueryResultInputIterator>
std::set<std::string> merge_unknown_words(QueryResultInputIterator first,
                                          QueryResultInputIterator last) {
  std::set<std::string> words;
  while (first != last) {
    words.insert(first->unknown_word().begin(), first->unknown_word().end());
    ++first;
  }
  return words;
}

template <typename QueryResultContainer>
std::set<std::string> merge_unknown_words(const QueryResultContainer& results) {
  return merge_unknown_words(results.begin(), results.end());
}

inline void tag_phrase(generated::Phrase& phrase,
                       const generated::Query& normalized_query) {
  aitools::check(phrase.word_size() == normalized_query.unit_size(), __func__);

  generated::Phrase::Word* word(nullptr);
  for (int i = 0; i < normalized_query.unit_size(); ++i) {
    word = phrase.mutable_word(i);
    switch (normalized_query.unit(i).tag()) {
      case generated::Query::Unit::QMARK:
        word->set_tag(generated::Phrase::Word::WORD_FOR_QMARK);
        break;
      case generated::Query::Unit::QMARK_FOR_ASTERISK:
        word->set_tag(generated::Phrase::Word::WORD_FOR_ASTERISK);
        break;
      case generated::Query::Unit::QMARK_FOR_PLUS:
        word->set_tag(generated::Phrase::Word::WORD_FOR_PLUS);
        break;
      case generated::Query::Unit::WORD:
        word->set_tag(generated::Phrase::Word::WORD);
        break;
      case generated::Query::Unit::WORD_IN_DICTSET:
        word->set_tag(generated::Phrase::Word::WORD_IN_DICTSET);
        break;
      case generated::Query::Unit::WORD_IN_OPTIONSET:
        word->set_tag(generated::Phrase::Word::WORD_IN_OPTIONSET);
        break;
      case generated::Query::Unit::WORD_IN_ORDERSET:
        word->set_tag(generated::Phrase::Word::WORD_IN_ORDERSET);
        break;
      case generated::Query::Unit::WORD_FOR_REGEX:
        word->set_tag(generated::Phrase::Word::WORD_FOR_REGEX);
        break;
      case generated::Query::Unit::WORD_FOR_REGEX_IN_OPTIONSET:
        word->set_tag(generated::Phrase::Word::WORD_FOR_REGEX_IN_OPTIONSET);
        break;
      case generated::Query::Unit::WORD_FOR_REGEX_IN_ORDERSET:
        word->set_tag(generated::Phrase::Word::WORD_FOR_REGEX_IN_ORDERSET);
        break;
      default:
        aitools::check(false, "invalid unit tag",
                       normalized_query.unit(i).tag());
    }
  }
}

} // namespace netspeak

#endif // NETSPEAK_POSTPROCESSING_HPP
