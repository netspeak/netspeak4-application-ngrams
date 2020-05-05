
// Generated from Query.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"


namespace antlr4 {


class  QueryLexer : public antlr4::Lexer {
public:
  enum {
    QMARK_TOKEN = 1, PLUS_TOKEN = 2, ASTERISK_TOKEN = 3, ASSOCIATION_TOKEN = 4, 
    HASH_TOKEN = 5, LBRACE_TOKEN = 6, RBRACE_TOKEN = 7, LBRACKET_TOKEN = 8, 
    RBRACKET_TOKEN = 9, DBLQUOTE_TOKEN = 10, REGEXWORD_TOKEN = 11, WORD_TOKEN = 12, 
    BACKSLASH_TOKEN = 13, WHITESPACE_TOKEN = 14
  };

  QueryLexer(antlr4::CharStream *input);
  ~QueryLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace antlr4
