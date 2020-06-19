#ifndef NETSPEAK_TYPEDEFS_HPP
#define NETSPEAK_TYPEDEFS_HPP

#include "aitools/value/pair.hpp"

namespace netspeak {

// The type of a phrases frequency used by the phrase corpus (PhraseCorpus).
typedef uint64_t PhraseCorpusPhraseFreq;

// The type of a phrases frequency used by the phrase index (Searcher).
typedef uint32_t PhraseIndexPhraseFreq;

// The type of a phrases id.
typedef uint32_t PhraseId;

// The type of a word id.
typedef uint32_t WordId;

// The type of entries in the phrase index.
typedef aitools::value::pair<PhraseIndexPhraseFreq, PhraseId> PhraseIndexValue;

// The type of entries in the postlist index.
// Semantic: (index-of-phrase-index-postlist-entry, phrase-frequency)
typedef aitools::value::pair<uint32_t, PhraseIndexPhraseFreq>
    PostlistIndexValue;

/**
 * @brief Each unit of a Netspeak query is given a unique id to identify it
 * throughout the evaluation of the query. This is the type of that unique id.
 */
typedef uint32_t QueryUnitId;

} // namespace netspeak

#endif // NETSPEAK_TYPEDEFS_HPP
