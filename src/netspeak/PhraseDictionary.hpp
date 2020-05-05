#ifndef NETSPEAK_PHRASE_DICTIONARY_HPP
#define NETSPEAK_PHRASE_DICTIONARY_HPP

#include "aitools/bighashmap/BigHashMap.hpp"
#include "aitools/value/pair.hpp"

namespace netspeak {

typedef aitools::BigHashMap<aitools::value::pair<uint64_t, uint32_t> >
    PhraseDictionary;

} // namespace netspeak

#endif // NETSPEAK_PHRASE_DICTIONARY_HPP
