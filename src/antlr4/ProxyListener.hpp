#ifndef PROXYLISTENER_HPP
#define PROXYLISTENER_HPP

#include <vector>

#include "antlr4/generated/QueryBaseListener.h"
#include "antlr4/generated/QueryListener.h"

#include "netspeak/error.hpp"

namespace antlr4 {

/**
 * @brief The ProxyListener class
 * This class allows multiply QueryListener to be used in the
 *  traversal of QueryParseTree. (Not Supported by default)
 *
 * The Listener functions are called in the same order the Listeners
 * have joined the std::vector.
 *
 * As a secondary feature a Listener can influence the progress, if it
 * has also access to std::shared_ptr<std::vector<QueryListener*>> object.
 * (for e.g. secondary error recognizer)
 */
class ProxyListener : public QueryBaseListener {
private:
  std::vector<QueryListener *>
      listener; /**< vector with all QueryListeners which always run*/
  std::vector<QueryListener *>
      observers; /**<vector with all QueryListeners to use if valid is true*/
  std::string errorMessages;
  bool valid = true;


public:
  /**
   * @brief ProxyListener
   * @param valid
   */
  ProxyListener();

  void notValid() {
    valid = false;
  }


  /**
   * @brief addListener
   * @param listener
   *
   * Add a Listener to the vector.
   */
  void addListener(QueryListener *listener, bool alwaysListen);

  /**
   * @brief addAllListener
   * @param listenerList
   *
   * Add all Listeners to the vector.
   */
  void addAllListener(std::vector<QueryListener *> listenerList,
                      bool alwaysListen);

  /**
   * @brief removeListener
   * @param listener
   * @return true if an element is removed
   *
   * Removes the first found element of the vector.
   */
  bool removeListener(QueryListener *listener, bool alwaysListen);

  /**
   * @brief removeAllListener
   * Removes all Listeners.
   */
  void removeAllListener(bool alwaysListen);

  std::vector<QueryListener *> getListener(bool alwaysListen);


  /**
   * @brief enterQuery
   * @param ctx
   *
   * Is called when entering a query node.
   */
  void enterQuery(QueryParser::QueryContext *ctx) override;

  /**
   * @brief exitQuery
   * @param ctx
   *
   * Is called when exiting a query node.
   */
  void exitQuery(QueryParser::QueryContext *ctx) override;

  /**
   * @brief enterUnits
   * @param ctx
   *
   * Is called when entering a units node.
   */
  void enterUnits(QueryParser::UnitsContext *ctx) override;
  /**
   * @brief exitUnits
   * @param ctx
   *
   * Is called when exiting a units node.
   */
  void exitUnits(QueryParser::UnitsContext *ctx) override;

  /**
   * @brief enterUnit
   * @param ctx
   *
   * Is called when entering a unit node.
   */
  void enterUnit(QueryParser::UnitContext *ctx) override;
  /**
   * @brief exitUnit
   * @param ctx
   *
   * Is called when exiting a unit node.
   */
  void exitUnit(QueryParser::UnitContext *ctx) override;

  /**
   * @brief enterOptionset
   * @param ctx
   *
   * Is called when entering an optionset node.
   */
  void enterOptionset(QueryParser::OptionsetContext *ctx) override;

  /**
   * @brief exitOptionset
   * @param ctx
   *
   * Is called when exiting an optionset node.
   */
  void exitOptionset(QueryParser::OptionsetContext *ctx) override;

  /**
   * @brief enterOrderset
   * @param ctx
   *
   * Is called when entering an orderset node.
   */
  void enterOrderset(QueryParser::OrdersetContext *ctx) override;

  /**
   * @brief exitOrderset
   * @param ctx
   *
   * Is called when exiting an orderset node.
   */
  void exitOrderset(QueryParser::OrdersetContext *ctx) override;

  /**
   * @brief enterDictset
   * @param ctx
   *
   * Is called when entering a dictset node.
   */
  void enterDictset(QueryParser::DictsetContext *ctx) override;

  /**
   * @brief exitDictset
   * @param ctx
   *
   * Is called when exiting a dictset node.
   */
  void exitDictset(QueryParser::DictsetContext *ctx) override;

  /**
   * @brief enterPhrase
   * @param ctx
   *
   * Is called when entering a phrase node.
   */
  void enterPhrase(QueryParser::PhraseContext *ctx) override;

  /**
   * @brief exitPhrase
   * @param ctx
   *
   * Is called when exiting a phrase node.
   */
  void exitPhrase(QueryParser::PhraseContext *ctx) override;

  /**
   * @brief enterAsterisk
   * @param ctx
   *
   * Is called when entering an asterisk node.
   */
  void enterAsterisk(QueryParser::AsteriskContext *ctx) override;

  /**
   * @brief exitAsterisk
   * @param ctx
   *
   * Is called when exiting an asterisk node.
   */
  void exitAsterisk(QueryParser::AsteriskContext *ctx) override;

  /**
   * @brief enterRegexword
   * @param ctx
   *
   * Is called when entering a regexword node.
   */
  void enterRegexword(QueryParser::RegexwordContext *ctx) override;

  /**
   * @brief exitRegexword
   * @param ctx
   *
   * Is called when exiting a regexword node.
   */
  void exitRegexword(QueryParser::RegexwordContext *ctx) override;

  /**
   * @brief enterWord
   * @param ctx
   *
   * Is called when entering a word node.
   */
  void enterWord(QueryParser::WordContext *ctx) override;

  /**
   * @brief exitWord
   * @param ctx
   *
   * Is called when exiting a word node.
   */
  void exitWord(QueryParser::WordContext *ctx) override;


  /**
   * @brief enterQmark
   * @param ctx
   *
   * Is called when entering a qmark node.
   */
  void enterQmark(QueryParser::QmarkContext *ctx) override;

  /**
   * @brief exitQmark
   * @param ctx
   *
   * Is called when exiting a qmark node.
   */
  void exitQmark(QueryParser::QmarkContext *ctx) override;

  /**
   * @brief enterPlus
   * @param ctx
   *
   * Is called when entering a plus node.
   */
  void enterPlus(QueryParser::PlusContext *ctx) override;

  /**
   * @brief exitPlus
   * @param ctx
   *
   * Is called when exiting a plus node.
   */
  void exitPlus(QueryParser::PlusContext *ctx) override;

  /**
   * @brief enterSplit
   * @param ctx
   *
   * Is called when entering a split node.
   */
  void enterAssociation(QueryParser::AssociationContext *ctx) override;

  /**
   * @brief exitSplit
   * @param ctx
   *
   * Is called when exiting a split node.
   */
  void exitAssociation(QueryParser::AssociationContext *ctx) override;

  /**
   * @brief enterEveryRule
   * @param ctx
   * Is called when entering any node.
   */
  void enterEveryRule(antlr4::ParserRuleContext *ctx) override;

  /**
   * @brief exitEveryRule
   * @param ctx
   *
   * Is called when exiting any node.
   */
  void exitEveryRule(antlr4::ParserRuleContext *ctx) override;


  // Missing documentation in antlr4. Not sure what they do...
  void visitTerminal(antlr4::tree::TerminalNode *node) override;
  void visitErrorNode(antlr4::tree::ErrorNode *node) override;
};

} // namespace antlr4

#endif // PROXYLISTENER_HPP
