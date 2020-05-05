
// Generated from Query.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "QueryParser.h"


namespace antlr4 {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by QueryParser.
 */
class  QueryVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by QueryParser.
   */
    virtual antlrcpp::Any visitQuery(QueryParser::QueryContext *context) = 0;

    virtual antlrcpp::Any visitUnits(QueryParser::UnitsContext *context) = 0;

    virtual antlrcpp::Any visitUnit(QueryParser::UnitContext *context) = 0;

    virtual antlrcpp::Any visitOptionset(QueryParser::OptionsetContext *context) = 0;

    virtual antlrcpp::Any visitOrderset(QueryParser::OrdersetContext *context) = 0;

    virtual antlrcpp::Any visitDictset(QueryParser::DictsetContext *context) = 0;

    virtual antlrcpp::Any visitPhrase(QueryParser::PhraseContext *context) = 0;

    virtual antlrcpp::Any visitAsterisk(QueryParser::AsteriskContext *context) = 0;

    virtual antlrcpp::Any visitRegexword(QueryParser::RegexwordContext *context) = 0;

    virtual antlrcpp::Any visitWord(QueryParser::WordContext *context) = 0;

    virtual antlrcpp::Any visitQmark(QueryParser::QmarkContext *context) = 0;

    virtual antlrcpp::Any visitPlus(QueryParser::PlusContext *context) = 0;

    virtual antlrcpp::Any visitAssociation(QueryParser::AssociationContext *context) = 0;


};

}  // namespace antlr4
