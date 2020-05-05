
// Generated from Query.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "QueryParser.h"


namespace antlr4 {

/**
 * This interface defines an abstract listener for a parse tree produced by QueryParser.
 */
class  QueryListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterQuery(QueryParser::QueryContext *ctx) = 0;
  virtual void exitQuery(QueryParser::QueryContext *ctx) = 0;

  virtual void enterUnits(QueryParser::UnitsContext *ctx) = 0;
  virtual void exitUnits(QueryParser::UnitsContext *ctx) = 0;

  virtual void enterUnit(QueryParser::UnitContext *ctx) = 0;
  virtual void exitUnit(QueryParser::UnitContext *ctx) = 0;

  virtual void enterOptionset(QueryParser::OptionsetContext *ctx) = 0;
  virtual void exitOptionset(QueryParser::OptionsetContext *ctx) = 0;

  virtual void enterOrderset(QueryParser::OrdersetContext *ctx) = 0;
  virtual void exitOrderset(QueryParser::OrdersetContext *ctx) = 0;

  virtual void enterDictset(QueryParser::DictsetContext *ctx) = 0;
  virtual void exitDictset(QueryParser::DictsetContext *ctx) = 0;

  virtual void enterPhrase(QueryParser::PhraseContext *ctx) = 0;
  virtual void exitPhrase(QueryParser::PhraseContext *ctx) = 0;

  virtual void enterAsterisk(QueryParser::AsteriskContext *ctx) = 0;
  virtual void exitAsterisk(QueryParser::AsteriskContext *ctx) = 0;

  virtual void enterRegexword(QueryParser::RegexwordContext *ctx) = 0;
  virtual void exitRegexword(QueryParser::RegexwordContext *ctx) = 0;

  virtual void enterWord(QueryParser::WordContext *ctx) = 0;
  virtual void exitWord(QueryParser::WordContext *ctx) = 0;

  virtual void enterQmark(QueryParser::QmarkContext *ctx) = 0;
  virtual void exitQmark(QueryParser::QmarkContext *ctx) = 0;

  virtual void enterPlus(QueryParser::PlusContext *ctx) = 0;
  virtual void exitPlus(QueryParser::PlusContext *ctx) = 0;

  virtual void enterAssociation(QueryParser::AssociationContext *ctx) = 0;
  virtual void exitAssociation(QueryParser::AssociationContext *ctx) = 0;


};

}  // namespace antlr4
