#ifndef NETSPEAK_QUERY_NORMALIZATION_HPP
#define NETSPEAK_QUERY_NORMALIZATION_HPP

#include <set>
#include <string>
#include <vector>

#include "netspeak/Dictionaries.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/regex/RegexIndex.hpp"


namespace netspeak {


/**
 * \brief Punctuation characters treated to be separated on tokenization.
 */
struct punctuation {
  static const char comma = ',';
  static const char semicolon = ';';
  static const char excmark = '!';
  static const char qmark = '?';
};

/**
 * \brief Constructs a query object from a sequence of tokens. Throws
 * \c invalid_query if the tokens do not form a valid query.
 */
void parse(std::shared_ptr<generated::RawResponse> response,
           netspeak::Dictionaries::Map *dictSet,
           netspeak::regex::RegexIndex *regex_index,
           std::vector<generated::Query> *normQueries);


/**
 * \brief Finds the set of alternatives for the given word.
 * Note that a word in this case could be a phrase as well,
 * both as input and/or in the output set.
 */
const std::set<std::string> lookup_word(const std::string &word,
                                        const Dictionaries::Map &dict);


/**
 * \brief Extracts the longest substrings from the given query. A substring is
 * formed by the concatenation of contiguous tokens, that do not represent any
 * kind of wildcard.
 *
 * Example:
 *
 *    to be ? not to
 * -> to be
 * -> not to
 */
const std::vector<std::string> extract_longest_substrings(
    const generated::Query &query);


} // namespace netspeak

#endif // NETSPEAK_QUERY_NORMALIZATION_HPP
