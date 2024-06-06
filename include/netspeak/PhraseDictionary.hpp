#ifndef NETSPEAK_PHRASE_DICTIONARY_HPP
#define NETSPEAK_PHRASE_DICTIONARY_HPP

#include "bighashmap/BigHashMap.hpp"
#include "value/pair.hpp"

namespace netspeak {

typedef bighashmap::BigHashMap<value::pair<uint64_t, uint32_t> > PhraseDictionary;

} // namespace netspeak

#endif // NETSPEAK_PHRASE_DICTIONARY_HPP
