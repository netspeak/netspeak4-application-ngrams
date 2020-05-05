#ifndef QUERYNORMLISTENER_HPP
#define QUERYNORMLISTENER_HPP

#include <unordered_map>
#include <vector>

#include <boost/foreach.hpp>

#include "antlr4-runtime.h"
#include "antlr4/generated/QueryBaseListener.h"
#include "antlr4/standardMethodsAndValues.hpp"

#include "netspeak/Dictionaries.hpp"
#include "netspeak/error.hpp"
#include "netspeak/generated/NetspeakMessages.pb.h"
#include "netspeak/query_methods.hpp"
#include "netspeak/query_normalization.hpp"
#include "netspeak/regex/RegexIndex.hpp"

namespace antlr4 {

/**
 * @brief The QueryNormListener class
 *
 * Normalize a given query. That means it generates queries of a
 * specific length.
 */
class QueryNormListener : public QueryBaseListener {
private:
  /** @brief maximal number of words to find for a regular expression */
  uint32_t maxRegexMatches;
  /** @brief defines the minimal phrase length */
  size_t min_phrase_length;
  /** @brief defines the maximal phrase length */
  size_t max_phrase_length;

  /** @brief all normalized queries are stored here */
  std::vector<netspeak::generated::Query> *normQueries;
  /** @brief temporary vector between two normalization steps */
  std::vector<netspeak::generated::Query> partlyNormQueries;
  /** @brief general temporary vector */
  std::vector<netspeak::generated::Query> tempQueries;
  /** @brief stores units from phrases */
  std::vector<netspeak::generated::Query::Unit> phraseUnit;
  /** @brief stores objects for later permutation (Orderset) */
  std::vector<std::vector<netspeak::generated::Query::Unit>> permutation;
  /** @brief general unit to avoid too much data */
  netspeak::generated::Query::Unit unit;

  /** @brief pointer to the synonym dictionary */
  netspeak::Dictionaries::Map *dict;
  /** @brief The regex index used to query regex words */
  netspeak::regex::RegexIndex *regex_index;
  std::unordered_map<std::string, std::vector<std::string>> regex_match_cache;

  bool inOptionSet;
  bool inOrderSet;
  bool inDictSet;
  bool inPhrase;

public:
  /**
   * @brief QueryNormListener
   * @param dict pointer to the synonym dictionary
   * @param regex_index The regex index used to query regex words
   * @param maxRegexMatches maximal number of words to find for a regular
   * expression
   * @param normQueries all normalized queries are stored here
   */
  QueryNormListener(netspeak::Dictionaries::Map *dict,
                    netspeak::regex::RegexIndex *regex_index,
                    uint32_t maxRegexMatches,
                    std::vector<netspeak::generated::Query> *normQueries,
                    size_t min_phrase_length, size_t max_phrase_length);

  /**
   * @brief enterUnits
   *
   * generates a new empty Query and stores it in partlyNormQueriest
   */
  void enterUnits(QueryParser::UnitsContext *) override;

  /**
   * @brief exitUnits
   *
   * push all Queries from partlyNormQueries in normQueries when their
   * amount of unit objects is less than MAX_PHRASE_LENGTH.
   * the rest will be delete
   */
  void exitUnits(QueryParser::UnitsContext *) override;

  /**
   * @brief enterUnit
   *
   * push all queries from partlyNormQueries into tempQueries
   */
  void enterUnit(QueryParser::UnitContext *) override;

  /**
   * @brief exitUnit
   *
   * delete all queries in tempQueries
   */
  void exitUnit(QueryParser::UnitContext *) override;


  /**
   * @brief enterOptionset
   *
   * sets inOptionSet to true
   */
  void enterOptionset(QueryParser::OptionsetContext *) override;

  /**
   * @brief exitOptionset
   *
   * sets inOptionSet to false
   *
   * if there is less than one child in this optionset all
   * queries in tempQueries are pushed into partlyNormQueries
   *
   */
  void exitOptionset(QueryParser::OptionsetContext *) override;

  /**
   * @brief enterOrderset
   *
   * set inOrderSet to true
   */
  void enterOrderset(QueryParser::OrdersetContext *) override;

  /**
   * @brief exitOrderset
   * @param ctx
   *
   * set inOptionSet to false
   *
   * the normalization of orderset is executed here:
   *
   * Steps:
   * 1. sort the vector permutation
   * 2. for each possible permutation do (uses std::next_permutation)
   * 3. add each permutation object (word, phrase, regexword) to a copy
   *    from each query in tempQueries
   * 4. if a regexword occurs, it will be replace with each found match for it
   * 5. each generate normalized Query is pushed into partlyNormQueries
   * 6. clears the vectors permutation, phraseUnit and tempQueries
   */
  void exitOrderset(QueryParser::OrdersetContext *ctx) override;

  /**
   * @brief enterDictset
   *
   * sets inDictSet to true
   */
  void enterDictset(QueryParser::DictsetContext *) override;

  /**
   * @brief exitDictset
   *
   * sets inDictSet to false
   */
  void exitDictset(QueryParser::DictsetContext *) override;

  /**
   * @brief enterPhrase
   *
   * sets inPhrase to true
   */
  void enterPhrase(QueryParser::PhraseContext *) override;

  /**
   * @brief exitPhrase
   *
   * sets inPhrase to false
   *
   * if inOrderSet:
   *   add phraseUnit to permutation
   *
   *   note cpp - reference for std::vector (concepts: CopyInsertable):
   *       If A is std::allocator<T>, then this will call
   *       placement-new, as by ::new((void*)p) T(v)
   *   (source : http://en.cppreference.com/w/cpp/concept/CopyInsertable)
   *
   * if inOptionSet:
   *   1. for each query in tempQueries add all units in phraseUnit
   *   2. if a unit is a regexword replace it with each found match.
   *   3. push all normalized queries into partlyNormQueries
   */
  void exitPhrase(QueryParser::PhraseContext *) override;


  /**
   * @brief enterAsterisk
   *
   * normalize the asterisk token:
   *
   * for each query in tempQueries
   *   adds qmarks to a query as long as it size is less than MAX_PHRASE_LENGTH
   *   all queries are saved and stored in partlyNormQueries (even when no qmark
   * is added)
   *
   */
  void enterAsterisk(QueryParser::AsteriskContext *) override;

  /**
   * @brief enterRegexword
   * @param ctx
   *
   * if the given regular expression is unknown in this query
   *   1. mutate the given regular expression to a normalized regular expression
   *   2. find up to maxRegexMatches matches or the Timeout occurs matches
   *
   * if inPhrase
   *   add the regex to phraseUnit
   * if inOrderSet
   *   add a vector with the regex to permutation
   * if inOptionSet
   *   add to each query in tempQueries each found regex match
   *   stores result in partlyNormQueries
   * else
   *   add to each query in tempQueries each found regex match
   *   stores result in partlyNormQueries
   */
  void enterRegexword(QueryParser::RegexwordContext *ctx) override;

  /**
   * @brief enterWord
   * @param ctx
   *
   * at first the word is transformed into a
   * standard form (delete all '\'
   *
   * if inPhrase
   *   add the word to phraseUnit
   * if inOrderSet
   *   add a vector with the word to permutation
   * if inOptionSet
   *   add to each query in tempQueries word
   *   stores result in partlyNormQueries
   * else
   *   add to each query in tempQueries word
   *   stores result in partlyNormQueries
   */
  void enterWord(QueryParser::WordContext *ctx) override;


  /**
   * @brief enterQmark
   *
   * add a qmark to each query in tempQueries and stores
   * the results in partlyNormQueries
   */
  void enterQmark(QueryParser::QmarkContext *) override;

  /**
   * @brief enterPlus
   *
   * for each query in tempQueries
   *   adds qmarks to a query as long as it sizeusing  is less than
   * MAX_PHRASE_LENGTH all queries are saved and stored in partlyNormQueries
   */
  void enterPlus(QueryParser::PlusContext *) override;

private:
  /**
   * @brief Returns all found matches of the given regex query.
   *
   * @param regex_query
   * @return std::vector<std::string>
   */
  std::vector<std::string> match_regex_query(std::string regex_query);
};

} // namespace antlr4
#endif // QUERYNORMLISTENER_HPP
