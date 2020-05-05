#ifndef QUERYLENGTHVERIFIER_HPP
#define QUERYLENTHVERIFIER_HPP

#include <limits>

#include "antlr4/generated/QueryBaseListener.h"
#include "antlr4/standardMethodsAndValues.hpp"

#include "netspeak/error.hpp"

namespace antlr4 {

/**
 * @brief The QueryLengthVerifier class
 *
 * Verify if the given query has at least one normalized form of
 * maximum MAX_PHRASE_LENGTH words.
 */
class QueryLengthVerifier : public QueryBaseListener {
private:
  int unitCount;   /**<minimum amount of words in a units node */
  int phraseCount; /**< minimum amount of words in a phrase node*/
  int counter;     /**< temporary counter*/

public:
  QueryLengthVerifier();

  /**
   * @brief exitUnits
   * @param ctx
   *
   * throws an invalid_query exception if unitCount is greater than
   * MAX_PHRASE_LENGTH
   */
  void exitUnits(QueryParser::UnitsContext *ctx) override;

  /**
   * @brief exitUnit
   *
   * add counter to unitCount
   */
  void exitUnit(QueryParser::UnitContext *) override;

  /**
   * @brief exitOptionset
   * @param ctx
   *
   *  determine the minimum amount of words in an optionset
   */
  void exitOptionset(QueryParser::OptionsetContext *ctx) override;

  /**
   * @brief enterPhrase
   * @param ctx
   *
   * determine the minimum amount of words in all phrases in a set
   */
  void enterPhrase(QueryParser::PhraseContext *ctx) override;
  /**
   * @brief enterRegexword
   * increase counter by 1
   */
  void enterRegexword(QueryParser::RegexwordContext *) override;
  /**
   * @brief enterWord
   * increase counter by 1
   */
  void enterWord(QueryParser::WordContext *) override;
  /**
   * @brief enterQmark
   * increase counter by 1
   */
  void enterQmark(QueryParser::QmarkContext *) override;
  /**
   * @brief enterPlus
   * increase counter by 1
   */
  void enterPlus(QueryParser::PlusContext *) override;
};

} // namespace antlr4

#endif // QueryLengthVERIFIER_HPP
