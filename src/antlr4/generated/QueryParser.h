
// Generated from Query.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"


namespace antlr4 {


class  QueryParser : public antlr4::Parser {
public:
  enum {
    QMARK_TOKEN = 1, PLUS_TOKEN = 2, ASTERISK_TOKEN = 3, ASSOCIATION_TOKEN = 4, 
    HASH_TOKEN = 5, LBRACE_TOKEN = 6, RBRACE_TOKEN = 7, LBRACKET_TOKEN = 8, 
    RBRACKET_TOKEN = 9, DBLQUOTE_TOKEN = 10, REGEXWORD_TOKEN = 11, WORD_TOKEN = 12, 
    BACKSLASH_TOKEN = 13, WHITESPACE_TOKEN = 14
  };

  enum {
    RuleQuery = 0, RuleUnits = 1, RuleUnit = 2, RuleOptionset = 3, RuleOrderset = 4, 
    RuleDictset = 5, RulePhrase = 6, RuleAsterisk = 7, RuleRegexword = 8, 
    RuleWord = 9, RuleQmark = 10, RulePlus = 11, RuleAssociation = 12
  };

  QueryParser(antlr4::TokenStream *input);
  ~QueryParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class QueryContext;
  class UnitsContext;
  class UnitContext;
  class OptionsetContext;
  class OrdersetContext;
  class DictsetContext;
  class PhraseContext;
  class AsteriskContext;
  class RegexwordContext;
  class WordContext;
  class QmarkContext;
  class PlusContext;
  class AssociationContext; 

  class  QueryContext : public antlr4::ParserRuleContext {
  public:
    QueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnitsContext *> units();
    UnitsContext* units(size_t i);
    antlr4::tree::TerminalNode *EOF();
    std::vector<AssociationContext *> association();
    AssociationContext* association(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryContext* query();

  class  UnitsContext : public antlr4::ParserRuleContext {
  public:
    UnitsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<UnitContext *> unit();
    UnitContext* unit(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnitsContext* units();

  class  UnitContext : public antlr4::ParserRuleContext {
  public:
    UnitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    WordContext *word();
    RegexwordContext *regexword();
    OptionsetContext *optionset();
    OrdersetContext *orderset();
    DictsetContext *dictset();
    QmarkContext *qmark();
    PlusContext *plus();
    AsteriskContext *asterisk();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnitContext* unit();

  class  OptionsetContext : public antlr4::ParserRuleContext {
  public:
    OptionsetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACKET_TOKEN();
    antlr4::tree::TerminalNode *RBRACKET_TOKEN();
    std::vector<RegexwordContext *> regexword();
    RegexwordContext* regexword(size_t i);
    std::vector<WordContext *> word();
    WordContext* word(size_t i);
    std::vector<PhraseContext *> phrase();
    PhraseContext* phrase(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionsetContext* optionset();

  class  OrdersetContext : public antlr4::ParserRuleContext {
  public:
    OrdersetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACE_TOKEN();
    antlr4::tree::TerminalNode *RBRACE_TOKEN();
    std::vector<RegexwordContext *> regexword();
    RegexwordContext* regexword(size_t i);
    std::vector<WordContext *> word();
    WordContext* word(size_t i);
    std::vector<PhraseContext *> phrase();
    PhraseContext* phrase(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrdersetContext* orderset();

  class  DictsetContext : public antlr4::ParserRuleContext {
  public:
    DictsetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HASH_TOKEN();
    WordContext *word();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DictsetContext* dictset();

  class  PhraseContext : public antlr4::ParserRuleContext {
  public:
    PhraseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> DBLQUOTE_TOKEN();
    antlr4::tree::TerminalNode* DBLQUOTE_TOKEN(size_t i);
    std::vector<RegexwordContext *> regexword();
    RegexwordContext* regexword(size_t i);
    std::vector<WordContext *> word();
    WordContext* word(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PhraseContext* phrase();

  class  AsteriskContext : public antlr4::ParserRuleContext {
  public:
    AsteriskContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASTERISK_TOKEN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsteriskContext* asterisk();

  class  RegexwordContext : public antlr4::ParserRuleContext {
  public:
    RegexwordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REGEXWORD_TOKEN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RegexwordContext* regexword();

  class  WordContext : public antlr4::ParserRuleContext {
  public:
    WordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WORD_TOKEN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WordContext* word();

  class  QmarkContext : public antlr4::ParserRuleContext {
  public:
    QmarkContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *QMARK_TOKEN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QmarkContext* qmark();

  class  PlusContext : public antlr4::ParserRuleContext {
  public:
    PlusContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLUS_TOKEN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PlusContext* plus();

  class  AssociationContext : public antlr4::ParserRuleContext {
  public:
    AssociationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASSOCIATION_TOKEN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssociationContext* association();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace antlr4
