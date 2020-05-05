
// Generated from Query.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "QueryVisitor.h"


namespace antlr4 {

/**
 * This class provides an empty implementation of QueryVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  QueryBaseVisitor : public QueryVisitor {
public:

  virtual antlrcpp::Any visitQuery(QueryParser::QueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnits(QueryParser::UnitsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnit(QueryParser::UnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOptionset(QueryParser::OptionsetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderset(QueryParser::OrdersetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDictset(QueryParser::DictsetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPhrase(QueryParser::PhraseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAsterisk(QueryParser::AsteriskContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRegexword(QueryParser::RegexwordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWord(QueryParser::WordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitQmark(QueryParser::QmarkContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPlus(QueryParser::PlusContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssociation(QueryParser::AssociationContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace antlr4
