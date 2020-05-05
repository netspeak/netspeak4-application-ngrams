
// Generated from Query.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "QueryListener.h"


namespace antlr4 {

/**
 * This class provides an empty implementation of QueryListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  QueryBaseListener : public QueryListener {
public:

  virtual void enterQuery(QueryParser::QueryContext * /*ctx*/) override { }
  virtual void exitQuery(QueryParser::QueryContext * /*ctx*/) override { }

  virtual void enterUnits(QueryParser::UnitsContext * /*ctx*/) override { }
  virtual void exitUnits(QueryParser::UnitsContext * /*ctx*/) override { }

  virtual void enterUnit(QueryParser::UnitContext * /*ctx*/) override { }
  virtual void exitUnit(QueryParser::UnitContext * /*ctx*/) override { }

  virtual void enterOptionset(QueryParser::OptionsetContext * /*ctx*/) override { }
  virtual void exitOptionset(QueryParser::OptionsetContext * /*ctx*/) override { }

  virtual void enterOrderset(QueryParser::OrdersetContext * /*ctx*/) override { }
  virtual void exitOrderset(QueryParser::OrdersetContext * /*ctx*/) override { }

  virtual void enterDictset(QueryParser::DictsetContext * /*ctx*/) override { }
  virtual void exitDictset(QueryParser::DictsetContext * /*ctx*/) override { }

  virtual void enterPhrase(QueryParser::PhraseContext * /*ctx*/) override { }
  virtual void exitPhrase(QueryParser::PhraseContext * /*ctx*/) override { }

  virtual void enterAsterisk(QueryParser::AsteriskContext * /*ctx*/) override { }
  virtual void exitAsterisk(QueryParser::AsteriskContext * /*ctx*/) override { }

  virtual void enterRegexword(QueryParser::RegexwordContext * /*ctx*/) override { }
  virtual void exitRegexword(QueryParser::RegexwordContext * /*ctx*/) override { }

  virtual void enterWord(QueryParser::WordContext * /*ctx*/) override { }
  virtual void exitWord(QueryParser::WordContext * /*ctx*/) override { }

  virtual void enterQmark(QueryParser::QmarkContext * /*ctx*/) override { }
  virtual void exitQmark(QueryParser::QmarkContext * /*ctx*/) override { }

  virtual void enterPlus(QueryParser::PlusContext * /*ctx*/) override { }
  virtual void exitPlus(QueryParser::PlusContext * /*ctx*/) override { }

  virtual void enterAssociation(QueryParser::AssociationContext * /*ctx*/) override { }
  virtual void exitAssociation(QueryParser::AssociationContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace antlr4
