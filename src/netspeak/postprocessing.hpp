#ifndef NETSPEAK_POSTPROCESSING_HPP
#define NETSPEAK_POSTPROCESSING_HPP

#include <iterator>
#include <list>
#include <set>

#include "aitools/util/check.hpp"

//#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/phrase_methods.hpp"
#include "netspeak/query_methods.hpp"

namespace netspeak {


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
