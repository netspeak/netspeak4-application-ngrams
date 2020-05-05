
// Generated from Query.g4 by ANTLR 4.7.1


#include "QueryListener.h"
#include "QueryVisitor.h"

#include "QueryParser.h"


using namespace antlrcpp;
using namespace antlr4;
using namespace antlr4;

QueryParser::QueryParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

QueryParser::~QueryParser() {
  delete _interpreter;
}

std::string QueryParser::getGrammarFileName() const {
  return "Query.g4";
}

const std::vector<std::string>& QueryParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& QueryParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- QueryContext ------------------------------------------------------------------

QueryParser::QueryContext::QueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<QueryParser::UnitsContext *> QueryParser::QueryContext::units() {
  return getRuleContexts<QueryParser::UnitsContext>();
}

QueryParser::UnitsContext* QueryParser::QueryContext::units(size_t i) {
  return getRuleContext<QueryParser::UnitsContext>(i);
}

tree::TerminalNode* QueryParser::QueryContext::EOF() {
  return getToken(QueryParser::EOF, 0);
}

std::vector<QueryParser::AssociationContext *> QueryParser::QueryContext::association() {
  return getRuleContexts<QueryParser::AssociationContext>();
}

QueryParser::AssociationContext* QueryParser::QueryContext::association(size_t i) {
  return getRuleContext<QueryParser::AssociationContext>(i);
}


size_t QueryParser::QueryContext::getRuleIndex() const {
  return QueryParser::RuleQuery;
}

void QueryParser::QueryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQuery(this);
}

void QueryParser::QueryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQuery(this);
}


antlrcpp::Any QueryParser::QueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitQuery(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::QueryContext* QueryParser::query() {
  QueryContext *_localctx = _tracker.createInstance<QueryContext>(_ctx, getState());
  enterRule(_localctx, 0, QueryParser::RuleQuery);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(26);
    units();
    setState(32);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == QueryParser::ASSOCIATION_TOKEN) {
      setState(27);
      association();
      setState(28);
      units();
      setState(34);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(35);
    match(QueryParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnitsContext ------------------------------------------------------------------

QueryParser::UnitsContext::UnitsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<QueryParser::UnitContext *> QueryParser::UnitsContext::unit() {
  return getRuleContexts<QueryParser::UnitContext>();
}

QueryParser::UnitContext* QueryParser::UnitsContext::unit(size_t i) {
  return getRuleContext<QueryParser::UnitContext>(i);
}


size_t QueryParser::UnitsContext::getRuleIndex() const {
  return QueryParser::RuleUnits;
}

void QueryParser::UnitsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnits(this);
}

void QueryParser::UnitsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnits(this);
}


antlrcpp::Any QueryParser::UnitsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitUnits(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::UnitsContext* QueryParser::units() {
  UnitsContext *_localctx = _tracker.createInstance<UnitsContext>(_ctx, getState());
  enterRule(_localctx, 2, QueryParser::RuleUnits);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(40);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << QueryParser::QMARK_TOKEN)
      | (1ULL << QueryParser::PLUS_TOKEN)
      | (1ULL << QueryParser::ASTERISK_TOKEN)
      | (1ULL << QueryParser::HASH_TOKEN)
      | (1ULL << QueryParser::LBRACE_TOKEN)
      | (1ULL << QueryParser::LBRACKET_TOKEN)
      | (1ULL << QueryParser::REGEXWORD_TOKEN)
      | (1ULL << QueryParser::WORD_TOKEN))) != 0)) {
      setState(37);
      unit();
      setState(42);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnitContext ------------------------------------------------------------------

QueryParser::UnitContext::UnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

QueryParser::WordContext* QueryParser::UnitContext::word() {
  return getRuleContext<QueryParser::WordContext>(0);
}

QueryParser::RegexwordContext* QueryParser::UnitContext::regexword() {
  return getRuleContext<QueryParser::RegexwordContext>(0);
}

QueryParser::OptionsetContext* QueryParser::UnitContext::optionset() {
  return getRuleContext<QueryParser::OptionsetContext>(0);
}

QueryParser::OrdersetContext* QueryParser::UnitContext::orderset() {
  return getRuleContext<QueryParser::OrdersetContext>(0);
}

QueryParser::DictsetContext* QueryParser::UnitContext::dictset() {
  return getRuleContext<QueryParser::DictsetContext>(0);
}

QueryParser::QmarkContext* QueryParser::UnitContext::qmark() {
  return getRuleContext<QueryParser::QmarkContext>(0);
}

QueryParser::PlusContext* QueryParser::UnitContext::plus() {
  return getRuleContext<QueryParser::PlusContext>(0);
}

QueryParser::AsteriskContext* QueryParser::UnitContext::asterisk() {
  return getRuleContext<QueryParser::AsteriskContext>(0);
}


size_t QueryParser::UnitContext::getRuleIndex() const {
  return QueryParser::RuleUnit;
}

void QueryParser::UnitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnit(this);
}

void QueryParser::UnitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnit(this);
}


antlrcpp::Any QueryParser::UnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitUnit(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::UnitContext* QueryParser::unit() {
  UnitContext *_localctx = _tracker.createInstance<UnitContext>(_ctx, getState());
  enterRule(_localctx, 4, QueryParser::RuleUnit);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(51);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case QueryParser::WORD_TOKEN: {
        enterOuterAlt(_localctx, 1);
        setState(43);
        word();
        break;
      }

      case QueryParser::REGEXWORD_TOKEN: {
        enterOuterAlt(_localctx, 2);
        setState(44);
        regexword();
        break;
      }

      case QueryParser::LBRACKET_TOKEN: {
        enterOuterAlt(_localctx, 3);
        setState(45);
        optionset();
        break;
      }

      case QueryParser::LBRACE_TOKEN: {
        enterOuterAlt(_localctx, 4);
        setState(46);
        orderset();
        break;
      }

      case QueryParser::HASH_TOKEN: {
        enterOuterAlt(_localctx, 5);
        setState(47);
        dictset();
        break;
      }

      case QueryParser::QMARK_TOKEN: {
        enterOuterAlt(_localctx, 6);
        setState(48);
        qmark();
        break;
      }

      case QueryParser::PLUS_TOKEN: {
        enterOuterAlt(_localctx, 7);
        setState(49);
        plus();
        break;
      }

      case QueryParser::ASTERISK_TOKEN: {
        enterOuterAlt(_localctx, 8);
        setState(50);
        asterisk();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptionsetContext ------------------------------------------------------------------

QueryParser::OptionsetContext::OptionsetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::OptionsetContext::LBRACKET_TOKEN() {
  return getToken(QueryParser::LBRACKET_TOKEN, 0);
}

tree::TerminalNode* QueryParser::OptionsetContext::RBRACKET_TOKEN() {
  return getToken(QueryParser::RBRACKET_TOKEN, 0);
}

std::vector<QueryParser::RegexwordContext *> QueryParser::OptionsetContext::regexword() {
  return getRuleContexts<QueryParser::RegexwordContext>();
}

QueryParser::RegexwordContext* QueryParser::OptionsetContext::regexword(size_t i) {
  return getRuleContext<QueryParser::RegexwordContext>(i);
}

std::vector<QueryParser::WordContext *> QueryParser::OptionsetContext::word() {
  return getRuleContexts<QueryParser::WordContext>();
}

QueryParser::WordContext* QueryParser::OptionsetContext::word(size_t i) {
  return getRuleContext<QueryParser::WordContext>(i);
}

std::vector<QueryParser::PhraseContext *> QueryParser::OptionsetContext::phrase() {
  return getRuleContexts<QueryParser::PhraseContext>();
}

QueryParser::PhraseContext* QueryParser::OptionsetContext::phrase(size_t i) {
  return getRuleContext<QueryParser::PhraseContext>(i);
}


size_t QueryParser::OptionsetContext::getRuleIndex() const {
  return QueryParser::RuleOptionset;
}

void QueryParser::OptionsetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptionset(this);
}

void QueryParser::OptionsetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptionset(this);
}


antlrcpp::Any QueryParser::OptionsetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitOptionset(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::OptionsetContext* QueryParser::optionset() {
  OptionsetContext *_localctx = _tracker.createInstance<OptionsetContext>(_ctx, getState());
  enterRule(_localctx, 6, QueryParser::RuleOptionset);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(53);
    match(QueryParser::LBRACKET_TOKEN);
    setState(59);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << QueryParser::DBLQUOTE_TOKEN)
      | (1ULL << QueryParser::REGEXWORD_TOKEN)
      | (1ULL << QueryParser::WORD_TOKEN))) != 0)) {
      setState(57);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case QueryParser::REGEXWORD_TOKEN: {
          setState(54);
          regexword();
          break;
        }

        case QueryParser::WORD_TOKEN: {
          setState(55);
          word();
          break;
        }

        case QueryParser::DBLQUOTE_TOKEN: {
          setState(56);
          phrase();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(61);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(62);
    match(QueryParser::RBRACKET_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrdersetContext ------------------------------------------------------------------

QueryParser::OrdersetContext::OrdersetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::OrdersetContext::LBRACE_TOKEN() {
  return getToken(QueryParser::LBRACE_TOKEN, 0);
}

tree::TerminalNode* QueryParser::OrdersetContext::RBRACE_TOKEN() {
  return getToken(QueryParser::RBRACE_TOKEN, 0);
}

std::vector<QueryParser::RegexwordContext *> QueryParser::OrdersetContext::regexword() {
  return getRuleContexts<QueryParser::RegexwordContext>();
}

QueryParser::RegexwordContext* QueryParser::OrdersetContext::regexword(size_t i) {
  return getRuleContext<QueryParser::RegexwordContext>(i);
}

std::vector<QueryParser::WordContext *> QueryParser::OrdersetContext::word() {
  return getRuleContexts<QueryParser::WordContext>();
}

QueryParser::WordContext* QueryParser::OrdersetContext::word(size_t i) {
  return getRuleContext<QueryParser::WordContext>(i);
}

std::vector<QueryParser::PhraseContext *> QueryParser::OrdersetContext::phrase() {
  return getRuleContexts<QueryParser::PhraseContext>();
}

QueryParser::PhraseContext* QueryParser::OrdersetContext::phrase(size_t i) {
  return getRuleContext<QueryParser::PhraseContext>(i);
}


size_t QueryParser::OrdersetContext::getRuleIndex() const {
  return QueryParser::RuleOrderset;
}

void QueryParser::OrdersetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrderset(this);
}

void QueryParser::OrdersetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrderset(this);
}


antlrcpp::Any QueryParser::OrdersetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitOrderset(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::OrdersetContext* QueryParser::orderset() {
  OrdersetContext *_localctx = _tracker.createInstance<OrdersetContext>(_ctx, getState());
  enterRule(_localctx, 8, QueryParser::RuleOrderset);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    match(QueryParser::LBRACE_TOKEN);
    setState(70);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << QueryParser::DBLQUOTE_TOKEN)
      | (1ULL << QueryParser::REGEXWORD_TOKEN)
      | (1ULL << QueryParser::WORD_TOKEN))) != 0)) {
      setState(68);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case QueryParser::REGEXWORD_TOKEN: {
          setState(65);
          regexword();
          break;
        }

        case QueryParser::WORD_TOKEN: {
          setState(66);
          word();
          break;
        }

        case QueryParser::DBLQUOTE_TOKEN: {
          setState(67);
          phrase();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(72);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(73);
    match(QueryParser::RBRACE_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DictsetContext ------------------------------------------------------------------

QueryParser::DictsetContext::DictsetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::DictsetContext::HASH_TOKEN() {
  return getToken(QueryParser::HASH_TOKEN, 0);
}

QueryParser::WordContext* QueryParser::DictsetContext::word() {
  return getRuleContext<QueryParser::WordContext>(0);
}


size_t QueryParser::DictsetContext::getRuleIndex() const {
  return QueryParser::RuleDictset;
}

void QueryParser::DictsetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDictset(this);
}

void QueryParser::DictsetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDictset(this);
}


antlrcpp::Any QueryParser::DictsetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitDictset(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::DictsetContext* QueryParser::dictset() {
  DictsetContext *_localctx = _tracker.createInstance<DictsetContext>(_ctx, getState());
  enterRule(_localctx, 10, QueryParser::RuleDictset);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(75);
    match(QueryParser::HASH_TOKEN);
    setState(76);
    word();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PhraseContext ------------------------------------------------------------------

QueryParser::PhraseContext::PhraseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> QueryParser::PhraseContext::DBLQUOTE_TOKEN() {
  return getTokens(QueryParser::DBLQUOTE_TOKEN);
}

tree::TerminalNode* QueryParser::PhraseContext::DBLQUOTE_TOKEN(size_t i) {
  return getToken(QueryParser::DBLQUOTE_TOKEN, i);
}

std::vector<QueryParser::RegexwordContext *> QueryParser::PhraseContext::regexword() {
  return getRuleContexts<QueryParser::RegexwordContext>();
}

QueryParser::RegexwordContext* QueryParser::PhraseContext::regexword(size_t i) {
  return getRuleContext<QueryParser::RegexwordContext>(i);
}

std::vector<QueryParser::WordContext *> QueryParser::PhraseContext::word() {
  return getRuleContexts<QueryParser::WordContext>();
}

QueryParser::WordContext* QueryParser::PhraseContext::word(size_t i) {
  return getRuleContext<QueryParser::WordContext>(i);
}


size_t QueryParser::PhraseContext::getRuleIndex() const {
  return QueryParser::RulePhrase;
}

void QueryParser::PhraseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPhrase(this);
}

void QueryParser::PhraseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPhrase(this);
}


antlrcpp::Any QueryParser::PhraseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitPhrase(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::PhraseContext* QueryParser::phrase() {
  PhraseContext *_localctx = _tracker.createInstance<PhraseContext>(_ctx, getState());
  enterRule(_localctx, 12, QueryParser::RulePhrase);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(78);
    match(QueryParser::DBLQUOTE_TOKEN);
    setState(83);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == QueryParser::REGEXWORD_TOKEN

    || _la == QueryParser::WORD_TOKEN) {
      setState(81);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case QueryParser::REGEXWORD_TOKEN: {
          setState(79);
          regexword();
          break;
        }

        case QueryParser::WORD_TOKEN: {
          setState(80);
          word();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(85);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(86);
    match(QueryParser::DBLQUOTE_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsteriskContext ------------------------------------------------------------------

QueryParser::AsteriskContext::AsteriskContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::AsteriskContext::ASTERISK_TOKEN() {
  return getToken(QueryParser::ASTERISK_TOKEN, 0);
}


size_t QueryParser::AsteriskContext::getRuleIndex() const {
  return QueryParser::RuleAsterisk;
}

void QueryParser::AsteriskContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAsterisk(this);
}

void QueryParser::AsteriskContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAsterisk(this);
}


antlrcpp::Any QueryParser::AsteriskContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitAsterisk(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::AsteriskContext* QueryParser::asterisk() {
  AsteriskContext *_localctx = _tracker.createInstance<AsteriskContext>(_ctx, getState());
  enterRule(_localctx, 14, QueryParser::RuleAsterisk);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(88);
    match(QueryParser::ASTERISK_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RegexwordContext ------------------------------------------------------------------

QueryParser::RegexwordContext::RegexwordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::RegexwordContext::REGEXWORD_TOKEN() {
  return getToken(QueryParser::REGEXWORD_TOKEN, 0);
}


size_t QueryParser::RegexwordContext::getRuleIndex() const {
  return QueryParser::RuleRegexword;
}

void QueryParser::RegexwordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRegexword(this);
}

void QueryParser::RegexwordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRegexword(this);
}


antlrcpp::Any QueryParser::RegexwordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitRegexword(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::RegexwordContext* QueryParser::regexword() {
  RegexwordContext *_localctx = _tracker.createInstance<RegexwordContext>(_ctx, getState());
  enterRule(_localctx, 16, QueryParser::RuleRegexword);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(90);
    match(QueryParser::REGEXWORD_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WordContext ------------------------------------------------------------------

QueryParser::WordContext::WordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::WordContext::WORD_TOKEN() {
  return getToken(QueryParser::WORD_TOKEN, 0);
}


size_t QueryParser::WordContext::getRuleIndex() const {
  return QueryParser::RuleWord;
}

void QueryParser::WordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWord(this);
}

void QueryParser::WordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWord(this);
}


antlrcpp::Any QueryParser::WordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitWord(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::WordContext* QueryParser::word() {
  WordContext *_localctx = _tracker.createInstance<WordContext>(_ctx, getState());
  enterRule(_localctx, 18, QueryParser::RuleWord);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(92);
    match(QueryParser::WORD_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QmarkContext ------------------------------------------------------------------

QueryParser::QmarkContext::QmarkContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::QmarkContext::QMARK_TOKEN() {
  return getToken(QueryParser::QMARK_TOKEN, 0);
}


size_t QueryParser::QmarkContext::getRuleIndex() const {
  return QueryParser::RuleQmark;
}

void QueryParser::QmarkContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQmark(this);
}

void QueryParser::QmarkContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQmark(this);
}


antlrcpp::Any QueryParser::QmarkContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitQmark(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::QmarkContext* QueryParser::qmark() {
  QmarkContext *_localctx = _tracker.createInstance<QmarkContext>(_ctx, getState());
  enterRule(_localctx, 20, QueryParser::RuleQmark);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(94);
    match(QueryParser::QMARK_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PlusContext ------------------------------------------------------------------

QueryParser::PlusContext::PlusContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::PlusContext::PLUS_TOKEN() {
  return getToken(QueryParser::PLUS_TOKEN, 0);
}


size_t QueryParser::PlusContext::getRuleIndex() const {
  return QueryParser::RulePlus;
}

void QueryParser::PlusContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPlus(this);
}

void QueryParser::PlusContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPlus(this);
}


antlrcpp::Any QueryParser::PlusContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitPlus(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::PlusContext* QueryParser::plus() {
  PlusContext *_localctx = _tracker.createInstance<PlusContext>(_ctx, getState());
  enterRule(_localctx, 22, QueryParser::RulePlus);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
    match(QueryParser::PLUS_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssociationContext ------------------------------------------------------------------

QueryParser::AssociationContext::AssociationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* QueryParser::AssociationContext::ASSOCIATION_TOKEN() {
  return getToken(QueryParser::ASSOCIATION_TOKEN, 0);
}


size_t QueryParser::AssociationContext::getRuleIndex() const {
  return QueryParser::RuleAssociation;
}

void QueryParser::AssociationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssociation(this);
}

void QueryParser::AssociationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<QueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssociation(this);
}


antlrcpp::Any QueryParser::AssociationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<QueryVisitor*>(visitor))
    return parserVisitor->visitAssociation(this);
  else
    return visitor->visitChildren(this);
}

QueryParser::AssociationContext* QueryParser::association() {
  AssociationContext *_localctx = _tracker.createInstance<AssociationContext>(_ctx, getState());
  enterRule(_localctx, 24, QueryParser::RuleAssociation);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
    match(QueryParser::ASSOCIATION_TOKEN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> QueryParser::_decisionToDFA;
atn::PredictionContextCache QueryParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN QueryParser::_atn;
std::vector<uint16_t> QueryParser::_serializedATN;

std::vector<std::string> QueryParser::_ruleNames = {
  "query", "units", "unit", "optionset", "orderset", "dictset", "phrase", 
  "asterisk", "regexword", "word", "qmark", "plus", "association"
};

std::vector<std::string> QueryParser::_literalNames = {
  "", "'?'", "'+'", "", "'|'", "'#'", "'{'", "'}'", "'['", "']'", "'\"'", 
  "", "", "'\\'"
};

std::vector<std::string> QueryParser::_symbolicNames = {
  "", "QMARK_TOKEN", "PLUS_TOKEN", "ASTERISK_TOKEN", "ASSOCIATION_TOKEN", 
  "HASH_TOKEN", "LBRACE_TOKEN", "RBRACE_TOKEN", "LBRACKET_TOKEN", "RBRACKET_TOKEN", 
  "DBLQUOTE_TOKEN", "REGEXWORD_TOKEN", "WORD_TOKEN", "BACKSLASH_TOKEN", 
  "WHITESPACE_TOKEN"
};

dfa::Vocabulary QueryParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> QueryParser::_tokenNames;

QueryParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x10, 0x67, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 
    0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x21, 0xa, 0x2, 0xc, 0x2, 
    0xe, 0x2, 0x24, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x7, 0x3, 0x29, 
    0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x2c, 0xb, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x36, 
    0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0x3c, 0xa, 
    0x5, 0xc, 0x5, 0xe, 0x5, 0x3f, 0xb, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 
    0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x47, 0xa, 0x6, 0xc, 0x6, 0xe, 
    0x6, 0x4a, 0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x54, 0xa, 0x8, 0xc, 0x8, 0xe, 
    0x8, 0x57, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 
    0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x2, 0x2, 0xf, 0x2, 0x4, 0x6, 0x8, 0xa, 
    0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x2, 0x2, 0x2, 0x6a, 0x2, 
    0x1c, 0x3, 0x2, 0x2, 0x2, 0x4, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x6, 0x35, 
    0x3, 0x2, 0x2, 0x2, 0x8, 0x37, 0x3, 0x2, 0x2, 0x2, 0xa, 0x42, 0x3, 0x2, 
    0x2, 0x2, 0xc, 0x4d, 0x3, 0x2, 0x2, 0x2, 0xe, 0x50, 0x3, 0x2, 0x2, 0x2, 
    0x10, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x12, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x14, 
    0x5e, 0x3, 0x2, 0x2, 0x2, 0x16, 0x60, 0x3, 0x2, 0x2, 0x2, 0x18, 0x62, 
    0x3, 0x2, 0x2, 0x2, 0x1a, 0x64, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x22, 0x5, 
    0x4, 0x3, 0x2, 0x1d, 0x1e, 0x5, 0x1a, 0xe, 0x2, 0x1e, 0x1f, 0x5, 0x4, 
    0x3, 0x2, 0x1f, 0x21, 0x3, 0x2, 0x2, 0x2, 0x20, 0x1d, 0x3, 0x2, 0x2, 
    0x2, 0x21, 0x24, 0x3, 0x2, 0x2, 0x2, 0x22, 0x20, 0x3, 0x2, 0x2, 0x2, 
    0x22, 0x23, 0x3, 0x2, 0x2, 0x2, 0x23, 0x25, 0x3, 0x2, 0x2, 0x2, 0x24, 
    0x22, 0x3, 0x2, 0x2, 0x2, 0x25, 0x26, 0x7, 0x2, 0x2, 0x3, 0x26, 0x3, 
    0x3, 0x2, 0x2, 0x2, 0x27, 0x29, 0x5, 0x6, 0x4, 0x2, 0x28, 0x27, 0x3, 
    0x2, 0x2, 0x2, 0x29, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x28, 0x3, 0x2, 
    0x2, 0x2, 0x2a, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x5, 0x3, 0x2, 0x2, 
    0x2, 0x2c, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x36, 0x5, 0x14, 0xb, 0x2, 
    0x2e, 0x36, 0x5, 0x12, 0xa, 0x2, 0x2f, 0x36, 0x5, 0x8, 0x5, 0x2, 0x30, 
    0x36, 0x5, 0xa, 0x6, 0x2, 0x31, 0x36, 0x5, 0xc, 0x7, 0x2, 0x32, 0x36, 
    0x5, 0x16, 0xc, 0x2, 0x33, 0x36, 0x5, 0x18, 0xd, 0x2, 0x34, 0x36, 0x5, 
    0x10, 0x9, 0x2, 0x35, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x35, 0x2e, 0x3, 0x2, 
    0x2, 0x2, 0x35, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x35, 0x30, 0x3, 0x2, 0x2, 
    0x2, 0x35, 0x31, 0x3, 0x2, 0x2, 0x2, 0x35, 0x32, 0x3, 0x2, 0x2, 0x2, 
    0x35, 0x33, 0x3, 0x2, 0x2, 0x2, 0x35, 0x34, 0x3, 0x2, 0x2, 0x2, 0x36, 
    0x7, 0x3, 0x2, 0x2, 0x2, 0x37, 0x3d, 0x7, 0xa, 0x2, 0x2, 0x38, 0x3c, 
    0x5, 0x12, 0xa, 0x2, 0x39, 0x3c, 0x5, 0x14, 0xb, 0x2, 0x3a, 0x3c, 0x5, 
    0xe, 0x8, 0x2, 0x3b, 0x38, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x39, 0x3, 0x2, 
    0x2, 0x2, 0x3b, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x3f, 0x3, 0x2, 0x2, 
    0x2, 0x3d, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x3, 0x2, 0x2, 0x2, 
    0x3e, 0x40, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x40, 
    0x41, 0x7, 0xb, 0x2, 0x2, 0x41, 0x9, 0x3, 0x2, 0x2, 0x2, 0x42, 0x48, 
    0x7, 0x8, 0x2, 0x2, 0x43, 0x47, 0x5, 0x12, 0xa, 0x2, 0x44, 0x47, 0x5, 
    0x14, 0xb, 0x2, 0x45, 0x47, 0x5, 0xe, 0x8, 0x2, 0x46, 0x43, 0x3, 0x2, 
    0x2, 0x2, 0x46, 0x44, 0x3, 0x2, 0x2, 0x2, 0x46, 0x45, 0x3, 0x2, 0x2, 
    0x2, 0x47, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x48, 0x46, 0x3, 0x2, 0x2, 0x2, 
    0x48, 0x49, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x4a, 
    0x48, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x4c, 0x7, 0x9, 0x2, 0x2, 0x4c, 0xb, 
    0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x7, 0x7, 0x2, 0x2, 0x4e, 0x4f, 0x5, 
    0x14, 0xb, 0x2, 0x4f, 0xd, 0x3, 0x2, 0x2, 0x2, 0x50, 0x55, 0x7, 0xc, 
    0x2, 0x2, 0x51, 0x54, 0x5, 0x12, 0xa, 0x2, 0x52, 0x54, 0x5, 0x14, 0xb, 
    0x2, 0x53, 0x51, 0x3, 0x2, 0x2, 0x2, 0x53, 0x52, 0x3, 0x2, 0x2, 0x2, 
    0x54, 0x57, 0x3, 0x2, 0x2, 0x2, 0x55, 0x53, 0x3, 0x2, 0x2, 0x2, 0x55, 
    0x56, 0x3, 0x2, 0x2, 0x2, 0x56, 0x58, 0x3, 0x2, 0x2, 0x2, 0x57, 0x55, 
    0x3, 0x2, 0x2, 0x2, 0x58, 0x59, 0x7, 0xc, 0x2, 0x2, 0x59, 0xf, 0x3, 
    0x2, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0x5, 0x2, 0x2, 0x5b, 0x11, 0x3, 0x2, 
    0x2, 0x2, 0x5c, 0x5d, 0x7, 0xd, 0x2, 0x2, 0x5d, 0x13, 0x3, 0x2, 0x2, 
    0x2, 0x5e, 0x5f, 0x7, 0xe, 0x2, 0x2, 0x5f, 0x15, 0x3, 0x2, 0x2, 0x2, 
    0x60, 0x61, 0x7, 0x3, 0x2, 0x2, 0x61, 0x17, 0x3, 0x2, 0x2, 0x2, 0x62, 
    0x63, 0x7, 0x4, 0x2, 0x2, 0x63, 0x19, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 
    0x7, 0x6, 0x2, 0x2, 0x65, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xb, 0x22, 0x2a, 
    0x35, 0x3b, 0x3d, 0x46, 0x48, 0x53, 0x55, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

QueryParser::Initializer QueryParser::_init;
