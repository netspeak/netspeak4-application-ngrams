#ifndef QUERYCOMPLEXITYMEASURELISTENER_HPP
#define QUERYCOMPLEXITYMEASURELISTENER_HPP

#include "antlr4/ProxyListener.hpp"
#include "antlr4/generated/QueryBaseListener.h"
#include "antlr4/standardMethodsAndValues.hpp"

namespace antlr4 {

/**
 * @brief The QueryComplexityMeasureListener class
 *
 * Computes the worst case complexity of a query.
 */
class QueryComplexityMeasureListener : public QueryBaseListener {
private:
  uint32_t totalComplexity = 0; /**<the current complextiy*/
  uint32_t temp = 1;
  ProxyListener *proxy;
  bool *valid;
  bool inOptionset = false;
  bool inOrderset = false;
  int maxRegexMatches; /**<maximal number of words to find for a regular
                          expression */

public:
  /**
   * @brief QueryComplexityMeasureListener
   * @param maxRegexMatches maximal number of words to find for a regular
   * expression
   * @param listenerList <shared_ptr<vector<QueryListener>> used in the
   * proxyListener
   */
  QueryComplexityMeasureListener(int maxRegexMatches, ProxyListener *proxy);


  /**
   * @brief exitQuery
   * @param ctx
   *
   * throws an invalid_query exception if totalComplexity is greater than
   * MAX_WORST_CASE_COMPLEXITY
   */
  void exitQuery(QueryParser::QueryContext *ctx) override;

  /**
   * @brief enterUnits
   *
   * tests if totalComplexity is greater than MAX_WORST_CASE_COMPLEXITY, if so,
   * all listeners in listenerList are removed except this one.
   */
  void enterUnits(QueryParser::UnitsContext *) override;

  /**
   * @brief exitUnits
   *
   * add temp to totalComplexity, set temp = 1
   */
  void exitUnits(QueryParser::UnitsContext *) override;

  /**
   * @brief exitUnit
   *
   * tests if temp is greater than MAX_WORST_CASE_COMPLEXITY, if so, all
   * listeners in listenerList are removed except this one.
   *
   */
  void exitUnit(QueryParser::UnitContext *) override;

  /**
   * @brief enterOptionset
   * @param ctx
   *
   * multiplies temp by the amount of children from ctx.
   */
  void enterOptionset(QueryParser::OptionsetContext *ctx) override;


  /**
   * @brief enterOrderset
   * @param ctx
   *
   * multiplies temp by faculty of the amount of children from ctx.
   */
  void enterOrderset(QueryParser::OrdersetContext *ctx) override;

  /**
   * @brief exitOrderset
   *
   * tests if temp is greater than MAX_WORST_CASE_COMPLEXITY, if so, all
   * listeners in listenerList are removed except this one.
   */
  void exitOrderset(QueryParser::OrdersetContext *) override;

  /**
   * @brief enterDictset
   *
   * multiplies temp by DICTSET_FACTOR
   */
  void enterDictset(QueryParser::DictsetContext *) override;

  /**
   * @brief enterAsterisk
   *
   * multiplies temp by ASTERISK_FACTOR
   */
  void enterAsterisk(QueryParser::AsteriskContext *) override;

  /**
   * @brief enterRegexword
   *
   * multiplies temp by maxRegexMatches
   */
  void enterRegexword(QueryParser::RegexwordContext *) override;

  /**
   * @brief enterWord
   *
   * multiplies temp by WORD_FACTOR
   */
  void enterWord(QueryParser::WordContext *) override;

  /**
   * @brief enterQmark
   *
   * multiplies temp by QMARK_FACTOR
   */
  void enterQmark(QueryParser::QmarkContext *) override;

  /**
   * @brief enterPlus
   *
   * multiplies temp by PLUS_FACTOR
   */
  void enterPlus(QueryParser::PlusContext *) override;
};

} // namespace antlr4

#endif // QUERYCOMPLEXITYMEASURELISTENER_HPP
